#include "../features.hpp"

float legit_bot::autowall::get_damage_multiplier(int hit_group) {
	switch (hit_group) {
	case hitgroup_head:
		return 4.0f;
	case hitgroup_stomach:
		return 1.25f;
	case hitgroup_leftleg:
	case hitgroup_rightleg:
		return 0.75f;
	default:
		return 1.0f;
	}
}

bool legit_bot::autowall::is_armored(int hit_group, bool helmet) {
	switch (hit_group) {
	case hitgroup_head:
		return helmet;

	case hitgroup_chest:
	case hitgroup_stomach:
	case hitgroup_leftarm:
	case hitgroup_rightarm:
		return true;
	default:
		return false;
	}
}

bool legit_bot::autowall::trace_to_exit(vec3_t& end, trace_t& tr, float x, float y, float z, float x2, float y2, float z2, trace_t* trace) {
	typedef bool(__fastcall* fn)(vec3_t&, trace_t&, float, float, float, float, float, float, trace_t*);
	static auto trace_to_exit = reinterpret_cast<fn>(utilities::pattern_scan("client.dll", sig_trace_to_exit));

	if (!trace_to_exit)
		return false;

	_asm {
		push trace
		push z2
		push y2
		push x2
		push z
		push y
		push x
		mov edx, tr
		mov ecx, end
		call trace_to_exit
		add esp, 0x1C
	}
}

static float legit_bot::autowall::handle_bullet_penetration(surface_data* enter_surface_data, trace_t& enter_trace, const vec3_t& direction, vec3_t& result, float penetration, float damage) {
	trace_t exit_trace;
	vec3_t dummy;

	if (!trace_to_exit(dummy, enter_trace, enter_trace.end.x, enter_trace.end.y, enter_trace.end.z, direction.x, direction.y, direction.z, &exit_trace))
		return -1.0f;

	auto exit_surface_data = interfaces::surface_props_physics->get_surface_data(exit_trace.surface.surfaceProps);

	float damage_modifier = 0.16f;
	float penetration_modifier = (enter_surface_data->penetrationmodifier + exit_surface_data->penetrationmodifier) / 2.0f;

	if (enter_surface_data->material == 71 || enter_surface_data->material == 89) {
		damage_modifier = 0.05f;
		penetration_modifier = 3.0f;
	}
	else if (enter_trace.contents >> 3 & 1 || enter_trace.surface.flags >> 7 & 1) {
		penetration_modifier = 1.0f;
	}

	if (enter_surface_data->material == exit_surface_data->material) {
		if (exit_surface_data->material == 85 || exit_surface_data->material == 87)
			penetration_modifier = 3.0f;
		else if (exit_surface_data->material == 76)
			penetration_modifier = 2.0f;
	}

	damage -= 11.25f / penetration / penetration_modifier + damage * damage_modifier + (exit_trace.end - enter_trace.end).length_sqr() / 24.0f / penetration_modifier;

	result = exit_trace.end;
	return damage;
}

static bool legit_bot::autowall::is_able_to_scan(player_t* local_player, entity_t* entity, const vec3_t& destination, const weapon_info_t* weapon_data, int min_damage) {
	float damage = static_cast<float>(weapon_data->damage);
	vec3_t start = local_player->get_eye_pos();
	vec3_t direction = (destination - start);
	direction /= direction.length();

	int hits_left = 4;
	static trace_t trace{};
	ray_t ray{};
	trace_filter filter{};

	while (damage >= 1.0f && hits_left) {
		filter.skip   = csgo::local_player;
		ray.initialize(start, destination);
		interfaces::trace_ray->trace_ray(ray, 0x4600400B, &filter, &trace);

		if (trace.flFraction == 1.0f)
			break;

		if (trace.entity == entity && trace.hit_group > hitgroup_generic&& trace.hit_group <= hitgroup_rightleg) {
			damage = get_damage_multiplier(trace.hit_group) * damage * powf(weapon_data->range_modifier, trace.flFraction * weapon_data->range / 500.0f);

			if (float armor_ratio{ weapon_data->armor_ratio / 2.0f }; is_armored(trace.hit_group, trace.entity->has_helmet()))
				damage -= (trace.entity->armor() < damage * armor_ratio / 2.0f ? trace.entity->armor() * 4.0f : damage) * (1.0f - armor_ratio);

			return damage >= min_damage;
		}
		const auto surface_data = interfaces::surface_props_physics->get_surface_data(trace.surface.surfaceProps);

		if (surface_data->penetrationmodifier < 0.1f)
			break;

		damage = autowall::handle_bullet_penetration(surface_data, trace, direction, start, weapon_data->penetration, damage);
		hits_left--;
	}
	return false;
}

void legit_bot::event(i_game_event* event) {
	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return;

	if (!csgo::local_player || !csgo::local_player->is_alive())
		return;

	auto event_name = utilities::fnv::hash(event->get_name());
	auto attacker = interfaces::entity_list->get_client_entity(interfaces::engine->get_player_for_user_id(event->get_int("attacker")));
	if (!attacker || !event_name)
		return;

	switch (event_name) {
	case utilities::fnv::hash("player_death"):
		if (attacker == csgo::local_player)
			legit_bot::kill_delay = interfaces::globals->tick_count + configuration::settings::aimbot_delay_after_kill;

		break;
	case utilities::fnv::hash("round_start"):
		legit_bot::kill_delay = 0;

		break;
	}
};

void legit_bot::aimbot(c_usercmd* cmd) {
	if (!csgo::local_player || !csgo::local_player->is_alive())
		return;

	auto weapon = csgo::local_player->active_weapon();

	if (!weapon || !weapon->clip1_count() || weapon->is_knife() || weapon->is_grenade() || weapon->is_c4())
		return;

	if (weapon->is_sniper() && !csgo::local_player->is_scoped())
		return;

	auto settings = configuration::settings::aimbot[weapon->item_definition_index()];

	if (!settings.enabled)
		return;

	auto weapon_data = weapon->get_weapon_data();

	if (!weapon_data)
		return;

	if (weapon->is_pistol() && weapon->next_primary_attack() > csgo::server_time() && configuration::settings::aimbot_auto_pistol)
		cmd->buttons &= ~in_attack;

	if (configuration::settings::aimbot_delay_after_kill > 1 && legit_bot::kill_delay >= interfaces::globals->tick_count)
		return;

	if (!(configuration::settings::aimbot_type > 0 ? 1 : cmd->buttons & in_attack))
		return;

	auto best_fov = settings.fov;
	auto best_target = vec3_t(0, 0, 0);
	auto local_eye_position = csgo::local_player->get_eye_pos();

	std::vector<int> bones;

	if (settings.hitboxes[0]) { //head
		bones.emplace_back(8); bones.emplace_back(7);
	}

	if (settings.hitboxes[1]) { //chest
		bones.emplace_back(6); bones.emplace_back(5); bones.emplace_back(4); bones.emplace_back(3);
	}

	if (settings.hitboxes[2]) { //hands
		bones.emplace_back(12); bones.emplace_back(40);
	}

	if (settings.hitboxes[3]) { //legs
		bones.emplace_back(65); bones.emplace_back(66); bones.emplace_back(72); bones.emplace_back(73);
	}

	static auto game_type = interfaces::console->get_convar("game_type");
	static auto weapon_recoil_scale = interfaces::console->get_convar("weapon_recoil_scale")->get_float();

	for (int i = 1; i <= interfaces::globals->max_clients; i++) {
		auto entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));

		if (!entity || entity == csgo::local_player || entity->dormant() || !entity->is_alive() || entity->has_gun_game_immunity()
			|| csgo::local_player->is_in_air() && !configuration::settings::aimbot_target_in_air)
			continue;

		if (game_type->get_int() == 6) {
			if ((csgo::local_player->survival_team() != -1) && (entity->survival_team() == csgo::local_player->survival_team()))
				continue;

			if (weapon->is_non_aim_weapon())
				continue;
		}
		else if (entity->team() == csgo::local_player->team() && !configuration::settings::aimbot_target_team)
			continue;

		for (const auto bone : bones) {
			auto bone_position = entity->get_bone_position(bone);

			if (!csgo::local_player->can_see_player_pos(entity, bone_position) && !configuration::settings::aimbot_target_visible || 
				!autowall::is_able_to_scan(csgo::local_player, entity, bone_position, weapon->get_weapon_data(), configuration::settings::aimbot_auto_wall_mim_damage)
				|| utilities::is_behind_smoke(local_eye_position, bone_position) && !configuration::settings::aimbot_target_smoke)
				continue;

			auto angle = math::calculate_angle(local_eye_position, bone_position, cmd->viewangles).clamped();
			angle -= (csgo::local_player->aim_punch_angle() * (weapon_recoil_scale * settings.rcs));

			auto fov = std::hypotf(angle.x, angle.y);
			if (fov < best_fov) {
				best_target = bone_position;
				best_fov = fov;
			}
		}
	}

	if (best_target.is_zero())
		return;

	auto angle = math::calculate_angle(local_eye_position, best_target, cmd->viewangles).clamped() /= settings.smooth;
	angle -= (csgo::local_player->aim_punch_angle() * (weapon_recoil_scale * settings.rcs)) /= settings.smooth;
	cmd->viewangles += angle.clamped();

	interfaces::engine->set_view_angles(cmd->viewangles);
}

bool legit_bot::hit_chance(vec3_t ang, player_t* player, float hitchance) {
	constexpr auto iterations = 150.f;

	auto weapon = csgo::local_player->active_weapon();

	if (!weapon)
		return false;

	vec3_t forward, right, up;
	const auto src = csgo::local_player->get_eye_pos();
	math::angle_vectors_alternative(ang, &forward, &right, &up);

	hitchance *= 100.f;

	auto c_hits = 0;
	const auto c_needed_hits = static_cast<int> (iterations * (hitchance / 100.f));

	weapon->update_accuracy_penalty();
	const auto weapon_spread = weapon->get_spread();
	const auto weapon_inaccuracy = weapon->get_inaccuracy();

	for (auto i = 0; i < static_cast<int> (iterations); i++) {
		auto a = math::random_float(0.f, 1.f);
		const auto b = math::random_float(0.f, 2.f * M_PI_F);
		auto c = math::random_float(0.f, 1.f);
		const auto d = math::random_float(0.f, 2.f * M_PI_F);

		if (weapon->item_definition_index() == WEAPON_REVOLVER) {
			a = 1.f - a * a;
			c = 1.f - c * c;
		}

		const auto inaccuracy = a * weapon_inaccuracy;
		const auto spread = c * weapon_spread;

		const vec3_t spread_view(cos(b) * inaccuracy + cos(d) * spread, sin(b) * inaccuracy + sin(d) * spread, 0);
		vec3_t direction;

		direction.x = forward.x + spread_view.x * right.x + spread_view.y * up.x;
		direction.y = forward.y + spread_view.x * right.y + spread_view.y * up.y;
		direction.z = forward.z + spread_view.x * right.z + spread_view.y * up.z;
		direction.normalized();
		
		vec3_t view_angles_spread;
		math::vector_angles(direction, up, view_angles_spread);
		math::normalize_in_place(view_angles_spread);

		vec3_t view_forward;
		math::angle_vectors(view_angles_spread, &view_forward);
		view_forward.normalized();

		view_forward = src + view_forward * weapon->get_weapon_data()->range;

		trace_t tr;
		ray_t ray;

		ray.initialize(src, view_forward);
		interfaces::trace_ray->clip_ray_to_entity(ray, MASK_SHOT | CONTENTS_GRATE, player, &tr);

		if (tr.entity == player)
			++c_hits;

		if (static_cast<int> (static_cast<float> (c_hits) / iterations * 100.f) > hitchance)
			return true;

		if (static_cast<int> (iterations) - i + c_hits < c_needed_hits)
			return false;
	}

	return false;
}

void legit_bot::trigger(c_usercmd* cmd) {
	if (!configuration::settings::triggerbot_enable)
		return;

	if (!configuration::settings::aimbot_trigger_type > 0 ? !GetAsyncKeyState(configuration::settings::aimbot_trigger_key) : 0)
		return;

	if (!csgo::local_player || !csgo::local_player->is_alive())
		return;

	auto weapon = csgo::local_player->active_weapon();

	if (!weapon || weapon->is_knife() || weapon->is_c4() || weapon->is_grenade() || !weapon->clip1_count() || weapon->next_primary_attack() > csgo::server_time())
		return;

	auto settings = configuration::settings::triggerbot[weapon->item_definition_index()];

	auto weapon_data = weapon->get_weapon_data();

	if (!weapon_data)
		return;

	vec3_t src{}, dst{}, forward{};
	trace_t tr{};
	ray_t ray{};
	trace_filter filter{};

	math::angle_vectors(cmd->viewangles, &forward);
	forward *= weapon->get_weapon_data()->range;
	filter.skip = csgo::local_player;
	src = csgo::local_player->get_eye_pos();
	dst = src + forward;
	ray.initialize(src, dst);
	interfaces::trace_ray->trace_ray(ray, 0x46004003, &filter, &tr);

	static auto weapon_recoil_scale = interfaces::console->get_convar("weapon_recoil_scale")->get_float();
	cmd->viewangles -= (csgo::local_player->aim_punch_angle() * weapon_recoil_scale);

	if (!tr.entity || !tr.entity->is_alive() || tr.entity->dormant() || tr.entity->has_gun_game_immunity() || csgo::local_player->team() == tr.entity->team() ||
		weapon->is_sniper() && !csgo::local_player->is_scoped() ||
		utilities::is_behind_smoke(csgo::local_player->get_eye_pos(), tr.entity->get_hitbox_position(hitbox_head)))
		return;

	auto chance = settings.hitchance / 100.f;

	if (!hit_chance(cmd->viewangles, tr.entity, chance))
		return;

	if (tr.hit_group == hitgroup_head ||
		tr.hit_group == hitgroup_chest ||
		tr.hit_group == hitgroup_stomach ||
		tr.hit_group == hitgroup_rightarm ||
		tr.hit_group == hitgroup_leftarm ||
		tr.hit_group == hitgroup_leftleg ||
		tr.hit_group == hitgroup_rightleg) {

		if (trigger_delay >= settings.delay) {
			trigger_delay = 0;
			cmd->buttons |= in_attack;
		}
		trigger_delay++;
	}
}