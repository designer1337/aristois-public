#include "../features.hpp"
#include "..//..//menu/menu.hpp"

void misc::logger::events(i_game_event* event) {
	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return;

	if (!event || !csgo::local_player)
		return;

	auto get_name = utilities::fnv::hash(event->get_name());

	if (get_name == utilities::fnv::hash("player_hurt")) {
		if (!configuration::settings::misc_event_logger[0])
			return;

		if (!csgo::local_player->is_alive())
			return;

		auto userid = interfaces::engine->get_player_for_user_id(event->get_int("userid"));

		if (!userid)
			return;

		auto attacker = interfaces::entity_list->get_client_entity(interfaces::engine->get_player_for_user_id(event->get_int("attacker")));

		if (!attacker || attacker != csgo::local_player)
			return;

		auto entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(userid));

		if (!entity)
			return;

		player_info_t info;
		interfaces::engine->get_player_info(entity->index(), &info);

		auto hitbox = event->get_int("hitgroup");
		auto damage = event->get_int("dmg_health");
		auto health = event->get_int("health");

		if (!damage || !health && health != 0)
			return;

		const auto hitgroup_to_name = [](int hit_group) {
			switch (hit_group) {
			case hitgroup_head:
				return "head";
			case hitgroup_leftleg:
				return "left leg";
			case hitgroup_rightleg:
				return "right leg";
			case hitgroup_stomach:
				return "stomach";
			case hitgroup_leftarm:
				return "left arm";
			case hitgroup_rightarm:
				return "right arm";
			default:
				return "body";
			}
		};

		auto hit_group = hitgroup_to_name(hitbox);

		if (!hit_group)
			return;

		std::string name = info.name;

		if (name.empty())
			return;

		if (name.length() > 15)
			name = name.substr(0, 15) + "...";

		std::transform(name.begin(), name.end(), name.begin(), ::tolower);

		misc::notification_system::notify((std::stringstream{ } << "hit " << name.c_str() << " in the " << hit_group << " for " << damage << " damage (" << health << " health remaining).").str().c_str(), color::white(configuration::settings::gui_menu_accent[3] * 255.f));
	}

	if (get_name == utilities::fnv::hash("item_purchase")) {
		if (!configuration::settings::misc_event_logger[1])
			return;

		auto userid = interfaces::engine->get_player_for_user_id(event->get_int("userid"));

		if (!userid)
			return;

		auto entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(userid));

		if (!entity)
			return;

		auto team = event->get_int("team");
		auto weapon = event->get_string("weapon");

		if (!team || !weapon)
			return;

		if (csgo::local_player->team() == event->get_int("team"))
			return;

		player_info_t info;
		interfaces::engine->get_player_info(entity->index(), &info);

		std::string name = info.name;

		if (name.empty())
			return;

		if (name.length() > 15)
			name = name.substr(0, 15) + "...";

		std::transform(name.begin(), name.end(), name.begin(), ::tolower);

		misc::notification_system::notify((std::stringstream{ } << name.c_str() << " bought " << weapon << ".").str().c_str(), color::white(configuration::settings::gui_menu_accent[3] * 255.f));
	}
}

void misc::notification_system::draw() {
	const auto stay_time = 5.f;

	for (auto i = 0; i < notify_list.size(); i++) {
		if (notify_list.empty())
			continue;

		auto notify = notify_list[i];

		if ((interfaces::globals->cur_time > notify.time + stay_time) || (notify_list.size() > 15)) {
			notify_list.erase(notify_list.begin() + i);
			i--;
			continue;
		}

		render::text(10, 10 + (i * 15), render::fonts::watermark, ("[aristois.me]"), false, color(configuration::settings::gui_menu_accent[0] * 255.f, configuration::settings::gui_menu_accent[1] * 255.f, configuration::settings::gui_menu_accent[2] * 255.f, configuration::settings::gui_menu_accent[3] * 255.f));
		render::text(95, 10 + (i * 15), render::fonts::watermark, notify.text.c_str(), false, notify._color);
	}
}

void misc::notification_system::notify(std::string text, color _color) {
	if (text.empty())
		return;

	notify_list.push_back(notify_t(text, _color));

	interfaces::console->console_color_printf({ static_cast<unsigned char>(configuration::settings::gui_menu_accent[0] * 255.f), 
		static_cast<unsigned char>(configuration::settings::gui_menu_accent[1] * 255.f),
		static_cast<unsigned char>(configuration::settings::gui_menu_accent[2] * 255.f), 255 }, ("[aristois.me] "));
	
	interfaces::console->console_printf(text.append(" \n").c_str());

}

void misc::correct_angles(int stage) {
	if (stage != FRAME_NET_UPDATE_POSTDATAUPDATE_START)
		return;

	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return;

	if (!csgo::local_player || !csgo::local_player->is_alive())
		return;

	for (int i = 1; i <= interfaces::globals->max_clients; i++) {
		auto entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));

		if (!entity || !entity->is_alive() || entity->dormant() || entity == csgo::local_player || entity->team() == csgo::local_player->team() || entity->is_moving())
			continue;

		auto anim_state = entity->get_anim_state();

		if (!anim_state)
			continue;

		if (entity->is_desyncing())
			anim_state->goal_feet_yaw = math::normalize_yaw(remainderf(entity->lower_body_yaw() + 180.f, 360.f));
	}
}

void misc::third_person() {
	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return;

	if (!csgo::local_player)
		return;

	if (csgo::local_player->is_alive()) {
		if (interfaces::input->in_thirdperson = GetKeyState(VK_XBUTTON2) ? true : false) {
			interfaces::input->camera_offset.z = configuration::settings::visuals_thirdperson_distance;
		}
	}

	if (!csgo::local_player->is_alive())
		csgo::local_player->observer_mode() = observer_modes::OBS_MODE_CHASE;
}

void misc::correct_local_animations(int stage) {
	if (!configuration::settings::aimbot_anti_hit_enable)
		return;

	if (stage != FRAME_RENDER_START)
		return;

	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return;

	if (!csgo::local_player || !csgo::local_player->is_alive() || !csgo::cmd)
		return;

	auto anim_state = csgo::local_player->get_anim_state();

	if (!anim_state)
		return;

	auto fresh_tick = []() {
		static int old_tick_count;

		if (old_tick_count != interfaces::globals->tick_count) {
			old_tick_count = interfaces::globals->tick_count;
			return true;
		}

		return false;
	};

	static auto proper_abs = anim_state->goal_feet_yaw;
	static auto sent_pose_params = csgo::local_player->pose_parameter();
	static anim_layer backup_layers[15];
	static int layer_count = 15;

	if (fresh_tick()) {
		std::memcpy(backup_layers, csgo::local_player->get_anim_overlays(), (sizeof(anim_layer) * layer_count));
		csgo::local_player->update_animation_state(anim_state, misc::break_lby ? csgo::send_packet_angles : csgo::cmd->viewangles);
		anim_state->last_client_side_animation_update_framecount = interfaces::globals->frame_count - 1;
		csgo::local_player->update_client_side_animations();

		if (csgo::send_packet) {
			proper_abs = anim_state->goal_feet_yaw;
			sent_pose_params = csgo::local_player->pose_parameter();
			csgo::real_angle = anim_state->goal_feet_yaw;
		}
	}
	csgo::local_player->set_abs_angles(vec3_t(0, proper_abs, 0));
	std::memcpy(csgo::local_player->get_anim_overlays(), backup_layers, (sizeof(anim_layer) * layer_count));
	csgo::local_player->pose_parameter() = sent_pose_params;
}

int misc::anti_hit::get_automatic_anti_hit_side(float yaw) {
	if (!configuration::settings::aimbot_anti_hit_enable)
		return false;

	if (!csgo::local_player || !csgo::local_player->is_alive())
		return false;

	float side_right, side_left;

	vec3_t src{}, dst{}, forward{}, right{}, up{};
	trace_t tr{};
	ray_t ray{}, ray1{};
	trace_filter filter{};

	vec3_t angles{};
	angles.x = 0;
	angles.y = yaw;

	math::angle_vectors_alternative(angles, &forward, &right, &up);

	filter.skip = csgo::local_player;
	src = csgo::local_player->get_eye_pos();
	dst = src + (forward * 384);

	ray.initialize(src + right * 35, dst + right * 35);

	interfaces::trace_ray->trace_ray(ray, MASK_SHOT, &filter, &tr);

	side_right = (tr.end - tr.start).length();

	ray1.initialize(src - right * 35, dst - right * 35);

	interfaces::trace_ray->trace_ray(ray1, MASK_SHOT, &filter, &tr);

	side_left = (tr.end - tr.start).length();

	static int result = 0;

	if (side_left > side_right)
		result = true;
	else if (side_right > side_left)
		result = false;

	return result;
}

void misc::anti_hit::update_lowerbody_breaker() {
	if (!configuration::settings::aimbot_anti_hit_enable)
		return;

	if (!csgo::local_player || !csgo::local_player->is_alive())
		return;

	float speed = csgo::local_player->velocity().length_2d_sqr();

	if (speed > 0.1)
		next_update = csgo::server_time() + 0.22;

	misc::break_lby = false;

	if (next_update <= csgo::server_time()) {
		next_update = csgo::server_time() + 1.1;
		misc::break_lby = true;
	}

	if (!(csgo::local_player->flags() & fl_onground))
		misc::break_lby = false;
}

void misc::anti_hit::run(c_usercmd* cmd, bool& send_packet) {
	if (!configuration::settings::aimbot_anti_hit_enable)
		return;

	if (!csgo::local_player || !csgo::local_player->is_alive())
		return;

	if (csgo::local_player->move_type() == movetype_ladder)
		return;

	if (cmd->buttons & in_attack)
		return;

	auto net_channel = interfaces::client_state->net_channel;

	if (!net_channel)
		return;

	send_packet = net_channel->choked_packets >= configuration::settings::aimbot_anti_hit_choked_packets;

	auto weapon = csgo::local_player->active_weapon();

	if (!weapon || weapon->is_grenade())
		return;

	if (GetAsyncKeyState(VK_LBUTTON) || GetAsyncKeyState(VK_RBUTTON) || (GetAsyncKeyState(0x45)))
		return;

	side = get_automatic_anti_hit_side(cmd->viewangles.y);

	if (break_lby) {
		send_packet = false;
		cmd->viewangles.y += side ? -120.f : 120.f;
	}

	cmd->viewangles.y += send_packet ? 0 : side ? -120.f : 120.f;

	cmd->viewangles.clamp();
}

void misc::anti_hit::draw_gui() {
	if (!configuration::settings::aimbot_anti_hit_enable || !configuration::settings::visuals_draw_anti_hit_lines)
		return;

	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return;

	if (!csgo::local_player || !csgo::local_player->is_alive())
		return;

	if (!csgo::local_player->get_desync_delta())
		return;;

	auto net_channel = interfaces::client_state->net_channel;

	if (!net_channel)
		return;

	delta = fabsf(csgo::local_player->get_desync_delta());

	auto window_size = ImVec2(150, 120);

	ImGui::SetNextWindowSize(window_size, ImGuiCond_Always);
	ImGui::SetNextWindowPos(ImVec2(30, 250), ImGuiCond_Once);
	ImGui::PushFont(menu::settings::tahoma3);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 10));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 1.0f);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.f, 0.f, 0.f, 0.f));
	ImGui::Begin(("indicators"), NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar); {
		auto choke_text = (std::stringstream{ } << "choked packets: " << std::to_string(net_channel->choked_packets).c_str());

		ImGui::Text(choke_text.str().c_str());
		ImGui::ProgressBar(0.2f * net_channel->choked_packets, ImVec2(-1, 0), "");

		ImGui::Text(side ? ("desync side: left") : ("desync side: right"));

		auto desync_delta_text = (std::stringstream{ } << "desync delta: " << std::setprecision(2) << (std::max)(delta, 0.0f));

		ImGui::Text(desync_delta_text.str().c_str());
		ImGui::ProgressBar(1.f * (delta / 58.f), ImVec2(-1, 0), "");
	}
	ImGui::End();
	ImGui::PopFont();
	ImGui::PopStyleVar(3);
	ImGui::PopStyleColor();
}

void misc::draw_spectators() {
	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return;

	if (!csgo::local_player || !csgo::local_player->is_alive())
		return;

	auto window_size = ImVec2(-1, -1);

	ImGui::SetNextWindowSize(window_size, ImGuiCond_Always);
	ImGui::SetNextWindowPos(ImVec2(30, 450), ImGuiCond_Once);
	ImGui::PushFont(menu::settings::tahoma3);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 10));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 1.0f);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.f, 0.f, 0.f, 0.f));
	ImGui::Begin(("spectators"), NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar); {
		const auto cur_window = ImGui::GetCurrentWindow();
		const ImVec2 w_pos = cur_window->Pos;

		ImGui::Text("spectators:");

		for (int i = 1; i <= interfaces::globals->max_clients; ++i) {
			auto entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));
			if (!entity || entity->dormant() || entity->is_alive() || entity->get_observer_target() != csgo::local_player)
				continue;

			player_info_t info;
			interfaces::engine->get_player_info(i, &info);

			if (!info.name)
				continue;

			if (info.ishltv)
				continue;

			std::string player_name = info.name;

			if (player_name.empty())
				continue;

			std::transform(player_name.begin(), player_name.end(), player_name.begin(), ::tolower);

			ImGui::Text(player_name.append(" \n").c_str());
		}

	}
	ImGui::End();
	ImGui::PopFont();
	ImGui::PopStyleVar(3);
	ImGui::PopStyleColor();
}

void misc::movement::bunny_hop(c_usercmd* cmd) {
	if (!configuration::settings::movement_bunnyhop)
		return;

	if (!csgo::local_player || !csgo::local_player->is_alive())
		return;

	if (csgo::local_player->move_type() == movetype_ladder || csgo::local_player->move_type() == movetype_noclip)
		return;

	static auto sv_autobunnyhopping = interfaces::console->get_convar("sv_autobunnyhopping");

	if (sv_autobunnyhopping->get_int())
		return;

	if ((std::rand() % 100 > configuration::settings::movement_bunnyhop_hitchance))
		return;

	static bool last_jumped = false, should_fake = false;

	if (!last_jumped && should_fake) {
		should_fake = false;
		cmd->buttons |= in_jump;
	}
	else if (cmd->buttons & in_jump) {
		if (csgo::local_player->flags() & fl_onground) {
			last_jumped = true;
			should_fake = true;
		}
		else {
			cmd->buttons &= ~in_jump;
			last_jumped = false;
		}
	}
	else {
		last_jumped = false;
		should_fake = false;
	}
};

void misc::movement::slide_walk(c_usercmd* cmd) {
	if (!configuration::settings::misc_slide_walk)
		return;

	if (!csgo::local_player || !csgo::local_player->is_alive())
		return;

	if (csgo::local_player->move_type() == move_type::movetype_ladder || csgo::local_player->move_type() == move_type::movetype_noclip)
		return;

	cmd->buttons ^= in_forward | in_back | in_moveleft | in_moveright;
}

void misc::kill_messages(i_game_event* event) {
	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return;

	if (!csgo::local_player || !csgo::local_player->is_alive())
		return;

	auto event_name = utilities::fnv::hash(event->get_name());
	auto attacker = interfaces::entity_list->get_client_entity(interfaces::engine->get_player_for_user_id(event->get_int("attacker")));
	auto entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_player_for_user_id(event->get_int("userid"))));

	if (!attacker || !entity || !event_name)
		return;

	switch (event_name) {
	case utilities::fnv::hash("player_death"):
		if (attacker == csgo::local_player && entity != csgo::local_player) {

			if (configuration::settings::misc_killspam)
				interfaces::engine->execute_cmd((std::string("say ").append(configuration::settings::misc_kill_message)).c_str());

			if (configuration::settings::misc_hitmarker_screen_effect)
				csgo::local_player->health_boost_time() = interfaces::globals->cur_time + 0.4f;
		}

		break;
	}
}

void misc::hitmarker::event(i_game_event* event) {
	if (!configuration::settings::misc_hitmarker)
		return;

	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return;

	if (!csgo::local_player || !csgo::local_player->is_alive())
		return;

	auto event_name = utilities::fnv::hash(event->get_name());
	auto attacker = interfaces::entity_list->get_client_entity(interfaces::engine->get_player_for_user_id(event->get_int("attacker")));
	if (!attacker || !event_name)
		return;

	switch (event_name) {
	case utilities::fnv::hash("player_hurt"):
		if (attacker == csgo::local_player) {
			hitmarker_time = 255;

			if (configuration::settings::misc_hitmarker_sound)
				interfaces::surface->play_sound("buttons\\arena_switch_press_02.wav");
		}
		break;
	}
}

void misc::hitmarker::draw() {
	if (!configuration::settings::misc_hitmarker)
		return;

	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return;

	if (!csgo::local_player || !csgo::local_player->is_alive())
		return;

	int width, height;
	interfaces::engine->get_screen_size(width, height);

	if (hitmarker_time > 0) {
		float alpha = hitmarker_time;

		render::line((width / 2) + 6, (height / 2) + 6, (width / 2) + 3, (height / 2) + 3, color::white(alpha));
		render::line((width / 2) - 6, (height / 2) + 6, (width / 2) - 3, (height / 2) + 3, color::white(alpha));
		render::line((width / 2) + 6, (height / 2) - 6, (width / 2) + 3, (height / 2) - 3, color::white(alpha));
		render::line((width / 2) - 6, (height / 2) - 6, (width / 2) - 3, (height / 2) - 3, color::white(alpha));

		hitmarker_time -= 2;
	}
}

void misc::remove_flash() {
	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return;

	if (!csgo::local_player || !csgo::local_player->is_alive())
		return;

	(configuration::settings::misc_remove_flashbang ? csgo::local_player->flash_alpha() = 255.0f - configuration::settings::misc_remove_flashbang_alpha : csgo::local_player->flash_alpha() = 255.0f);
}

void misc::remove_grass_danger_zone() {
	auto map_name = utilities::fnv::hash(interfaces::engine->get_level_name());
	const auto grass_blacksite = interfaces::material_system->find_material("detail/detailsprites_survival", nullptr),
		grass_sirocco = interfaces::material_system->find_material("detail/dust_massive_detail_sprites", nullptr),
		grass_junglety = interfaces::material_system->find_material("detail/tropical_grass", nullptr);

	switch (map_name) {
	case utilities::fnv::hash("dz_blacksite"):
		grass_blacksite->set_material_var_flag(material_var_no_draw, configuration::settings::visuals_danger_zone_items[0]);
		break;
	case utilities::fnv::hash("dz_sirocco"):
		grass_sirocco->set_material_var_flag(material_var_no_draw, configuration::settings::visuals_danger_zone_items[0]);
		break;
	case utilities::fnv::hash("dz_junglety"):
		grass_junglety->set_material_var_flag(material_var_no_draw, configuration::settings::visuals_danger_zone_items[0]);
		break;
	}
}

void misc::clantag_spammer() {
	if (!configuration::settings::misc_clantag_spammer)
		return;

	static int last_time = 0;
	if (static_cast<int>(interfaces::globals->cur_time) != last_time) {
		std::string clan_tag = configuration::settings::misc_clantag_text;

		if (clan_tag.empty())
			return;

		utilities::apply_clan_tag(configuration::settings::misc_synced_clantag ? clan_tag.append(" ").substr(0, static_cast<int>(interfaces::globals->cur_time) % clan_tag.length()).c_str() : clan_tag.c_str());

		last_time = static_cast<int>(interfaces::globals->cur_time);
	}
}

std::once_flag flag1;
void misc::name_spammer() {
	if (!configuration::settings::misc_name_spammer)
		return;

	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return;

	if (!csgo::local_player)
		return;

	player_info_t info;
	interfaces::engine->get_player_info(csgo::local_player->index(), &info);

	std::call_once(flag1, [&] { backup_local_player_name = info.name; });

	static auto last_time = 0.0f;
	if (interfaces::globals->cur_time - last_time < 0.4f)
		return;

	last_time = interfaces::globals->cur_time;

	counter++;

	switch (counter) {
	case 1: utilities::apply_name("Get"); break;
	case 2: utilities::apply_name("Get Good"); break;
	case 3: utilities::apply_name("Get Good, Get"); break;
	case 4: utilities::apply_name("Get Good, Get Aristois"); break;
	case 5: utilities::apply_name(backup_local_player_name.c_str()); break;
	case 6: break;
	}

	if (counter == 6) {
		counter = 0;
		configuration::settings::misc_name_spammer = 0;
	}
}

void misc::reveal_server_ranks(c_usercmd* cmd) {
	if (!configuration::settings::misc_reveal_server_ranks)
		return;

	if (cmd->buttons & in_score)
		interfaces::client->dispatch_user_message(cs_um_serverrankrevealall, 0, 0, nullptr);
}

void misc::sniper_force_crosshair() {
	static auto weapon_debug_spread_show = interfaces::console->get_convar("weapon_debug_spread_show");
	weapon_debug_spread_show->set_value(configuration::settings::misc_force_crosshair && !csgo::local_player->is_scoped() ? 3 : 0);
}

void misc::view_model_offsets() {
	if (!configuration::settings::visuals_view_model_changer_enable)
		return;

	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return;

	if (!csgo::local_player || !csgo::local_player->is_alive())
		return;

	const auto view_model = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity_handle(csgo::local_player->view_model()));

	if (!view_model)
		return;

	vec3_t forward{}, right{}, up{};
	math::angle_vectors_alternative(view_model->abs_angles(), &forward, &right, &up);

	view_model->set_abs_origin(view_model->abs_origin() + forward * configuration::settings::visuals_viewmodel_z + up * configuration::settings::visuals_viewmodel_y + right * configuration::settings::visuals_viewmodel_x);
}