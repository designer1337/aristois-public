#include "../features.hpp"

#pragma warning( disable : 4244 ) 

bool visuals::calculate_player_box(player_t* entity, visuals::box& in) {
	if (!entity)
		return false;

	auto trans = entity->coordinate_frame();

	auto collideable = entity->collideable();

	if (!collideable)
		return false;

	vec3_t min = collideable->mins();
	vec3_t max = collideable->maxs();

	vec3_t points[] = { vec3_t(min.x, min.y, min.z), vec3_t(min.x, max.y, min.z), vec3_t(max.x, max.y, min.z), vec3_t(max.x, min.y, min.z),
						vec3_t(max.x, max.y, max.z), vec3_t(min.x, max.y, max.z), vec3_t(min.x, min.y, max.z), vec3_t(max.x, min.y, max.z) };

	vec3_t points_transformed[8];
	for (int i = 0; i < 8; i++) {
		math::transform_vector(points[i], trans, points_transformed[i]);
	}

	vec3_t pos = entity->abs_origin(), flb, brt, blb, frt, frb, brb, blt, flt;

	if (!interfaces::debug_overlay->world_to_screen(points_transformed[3], flb) || !interfaces::debug_overlay->world_to_screen(points_transformed[5], brt)
		|| !interfaces::debug_overlay->world_to_screen(points_transformed[0], blb) || !interfaces::debug_overlay->world_to_screen(points_transformed[4], frt)
		|| !interfaces::debug_overlay->world_to_screen(points_transformed[2], frb) || !interfaces::debug_overlay->world_to_screen(points_transformed[1], brb)
		|| !interfaces::debug_overlay->world_to_screen(points_transformed[6], blt) || !interfaces::debug_overlay->world_to_screen(points_transformed[7], flt))
		return false;

	vec3_t arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };

	float left = flb.x;
	float top = flb.y;
	float right = flb.x;
	float bottom = flb.y;

	for (int i = 1; i < 8; i++) {
		if (left > arr[i].x)
			left = arr[i].x;
		if (bottom < arr[i].y)
			bottom = arr[i].y;
		if (right < arr[i].x)
			right = arr[i].x;
		if (top > arr[i].y)
			top = arr[i].y;
	}

	in.x = static_cast<int>(left);
	in.y = static_cast<int>(top) - 5;
	in.w = static_cast<int>(right) - static_cast<int>(left);
	in.h = static_cast<int>(bottom) - static_cast<int>(top) + 5;

	return true;
}

auto render_entity_box = [&](visuals::box _box, color _color, float alpha) {
	render::rect(_box.x - 1, _box.y - 1, _box.w + 2, _box.h + 2, color::black(alpha / 1.5f));
	render::rect(_box.x + 1, _box.y + 1, _box.w - 2, _box.h - 2, color::black(alpha / 1.5f));
	render::rect(_box.x, _box.y, _box.w, _box.h, _color);
};

//deadcell
void visuals::activation_type() {
	static bool is_down = false;
	static bool is_clicked = false;

	if (configuration::settings::visuals_toggle_type == 0) //always
		should_render = true;

	else if (configuration::settings::visuals_toggle_type == 1) { //on key
		should_render = GetAsyncKeyState(configuration::settings::visuals_toggle_key) ? true : false;
	}
	else if (configuration::settings::visuals_toggle_type == 2) { //toggle
		if (GetAsyncKeyState(configuration::settings::visuals_toggle_key)) {
			is_clicked = false;
			is_down = true;
		}
		else if (!GetAsyncKeyState(configuration::settings::visuals_toggle_key) && is_down) {
			is_clicked = true; is_down = false;
		}
		else {
			is_clicked = false; is_down = false;
		}

		if (is_clicked)
			should_render = !should_render;
	}

	//todo visible only / local player death / anti ss 
}

void visuals::player::run() {
	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return;

	if (!csgo::local_player)
		return;

	activation_type();

	if (!should_render)
		return;

	if (!configuration::settings::visuals_enable)
		return;

	for (int i = 1; i <= interfaces::globals->max_clients; i++) {
		auto entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));

		if (!entity || entity == csgo::local_player || !entity->is_alive())
			continue;

		visuals::box _box;
		if (!calculate_player_box(entity, _box))
			continue;

		static auto game_type = interfaces::console->get_convar("game_type");
		if (game_type->get_int() == 6) {
			if ((csgo::local_player->survival_team() != -1) && (entity->survival_team() == csgo::local_player->survival_team()))
				continue;
		}
		else if (entity->team() == csgo::local_player->team() && !configuration::settings::visuals_enable_team)
			continue;

		if (configuration::settings::visuals_force_radar)
			entity->spotted() = true;

		animate(entity, entity->index());
		name(entity, _box);
		box(entity, _box);
		health(entity, _box);
		weapon(entity, _box);
		armor(entity, _box);
		flags(entity, _box);

		if (configuration::settings::visuals_player_skeleton && configuration::settings::aimbot_backtrack_enable) {
			auto record = &lag_compensation::records[entity->index()];

			if (record && !record->empty() && record->size() && lag_compensation::valid(record->front().simulation_time))
				skeleton(entity, color::white(fade.at(entity->index())), record->back().matrix);
		}
	}
}

void visuals::player::animate(player_t * entity, const int index) {
	const float step = 255.f * (20.f / 15.f) * interfaces::globals->frame_time;

	configuration::settings::visuals_fade ? entity->dormant()
		? fade.at(index) -= step : fade.at(index) += step * 5.f : entity->dormant()
		? fade.at(index) = 0.f : fade.at(index) = 230;

	if (fade.at(index) > 230)
		fade.at(index) = 230;
	if (fade.at(index) < 0.f)
		fade.at(index) = 0.f;
}

void visuals::player::skeleton(player_t* entity, color _color, matrix_t matrix[256]) {
	auto get_bone_position = [&](int bone) -> vec3_t {
		return vec3_t(matrix[bone][0][3], matrix[bone][1][3], matrix[bone][2][3]);
	};

	auto model = interfaces::model_info->get_studio_model(entity->model());

	if (!model)
		return;

	constexpr int chest_bone = 6;
	vec3_t upper_direction = get_bone_position(chest_bone + 1) - get_bone_position(chest_bone),
		breast_bone = get_bone_position(chest_bone) + upper_direction / 2;
	vec3_t child, parent;

	for (int i = 0; i < model->bones_count; i++) {
		auto bone = model->bone(i);

		if (!bone)
			return;

		if ((bone->flags & BONE_USED_BY_HITBOX) && (bone->parent != -1)) {
			child = vec3_t(matrix[i][0][3], matrix[i][1][3], matrix[i][2][3]);
			parent = vec3_t(matrix[bone->parent][0][3], matrix[bone->parent][1][3], matrix[bone->parent][2][3]);

			vec3_t delta_child = child - breast_bone, delta_parent = parent - breast_bone;

			if ((delta_parent.length() < 9 && delta_child.length() < 9))
				parent = breast_bone;

			if (i == chest_bone - 1)
				child = breast_bone;

			if (abs(delta_child.z) < 5 && (delta_parent.length() < 5 && delta_child.length() < 5) || i == chest_bone)
				continue;

			vec3_t schild, sparent;
			if (!math::world_to_screen(child, schild) || !math::world_to_screen(parent, sparent))
				return;

			render::line(schild.x, schild.y, sparent.x, sparent.y, _color);
		}
	}
}

void visuals::player::name(player_t * entity, visuals::box _box) {
	if (!configuration::settings::visuals_player_name)
		return;

	player_info_t info;
	interfaces::engine->get_player_info(entity->index(), &info);

	std::string print(info.name);

	if (print.empty())
		return;

	if (print.length() > 15)
		print = print.substr(0, 15) + "...";

	std::transform(print.begin(), print.end(), print.begin(), ::tolower);

	render::text(_box.x + (_box.w / 2), _box.y - 13, render::fonts::visitor9, print, true, color(configuration::settings::visuals_name_color[0] * 255.f, configuration::settings::visuals_name_color[1] * 255.f, configuration::settings::visuals_name_color[2] * 255.f, fade.at(entity->index())));
}

void visuals::player::box(player_t * entity, visuals::box _box) {
	if (!configuration::settings::visuals_player_box)
		return;

	render::rect(_box.x - 1, _box.y - 1, _box.w + 2, _box.h + 2, color::black(fade.at(entity->index()) / 1.5f));
	render::rect(_box.x + 1, _box.y + 1, _box.w - 2, _box.h - 2, color::black(fade.at(entity->index()) / 1.5f));
	render::rect(_box.x, _box.y, _box.w, _box.h, color(configuration::settings::visuals_box_color[0] * 255.f, configuration::settings::visuals_box_color[1] * 255.f, configuration::settings::visuals_box_color[2] * 255.f, fade.at(entity->index())));
}

void visuals::player::flags(player_t* entity, visuals::box _box) {
	std::vector<std::pair<std::string, color>> flags;

	player_info_t info;
	interfaces::engine->get_player_info(entity->index(), &info);

	if (configuration::settings::visuals_flags[0] && info.fakeplayer)
		flags.push_back(std::pair<std::string, color>(("BOT"), color::red(fade.at(entity->index()))));

	if (configuration::settings::visuals_flags[1] && entity->has_helmet() && entity->armor() > 0)
		flags.push_back(std::pair<std::string, color>(("HK"), color::white(fade.at(entity->index()))));
	else if (configuration::settings::visuals_flags[1] && !entity->has_helmet() && entity->armor() > 0)
		flags.push_back(std::pair<std::string, color>(("K"), color::white(fade.at(entity->index()))));

	if (configuration::settings::visuals_flags[2] && entity->money())
		flags.push_back(std::pair<std::string, color>(std::string("$").append(std::to_string(entity->money())), color(120, 180, 10, fade.at(entity->index()))));

	if (configuration::settings::visuals_flags[3] && entity->is_scoped())
		flags.push_back(std::pair<std::string, color>(std::string(("ZOOM")), color(80, 160, 200, fade.at(entity->index()))));

	if (configuration::settings::visuals_flags[4] && entity->is_flashed())
		flags.push_back(std::pair<std::string, color>(std::string(("FLASHED")), color::white(fade.at(entity->index()))));

	if (configuration::settings::visuals_flags[5] && entity->is_desyncing() && !entity->is_moving())
		flags.push_back(std::pair<std::string, color>(std::string(("DESYNC")), color::red(fade.at(entity->index()))));

	if (configuration::settings::visuals_flags[6] && entity->is_defusing())
		flags.push_back(std::pair<std::string, color>(std::string(("DEFUSING")), color::yellow(fade.at(entity->index()))));

	auto position = 0;
	for (auto text : flags) {
		render::text(_box.x + _box.w + 3, _box.y + position - 2, render::fonts::name, text.first, false, text.second);
		position += 10;
	}
}

constexpr float anim_time = 255 / 2.0f;
void visuals::player::health(player_t * entity, visuals::box _box) {
	if (!configuration::settings::visuals_player_health)
		return;

	static float prev_player_hp[65];

	if (prev_player_hp[entity->index()] > entity->health())
		prev_player_hp[entity->index()] -= anim_time * interfaces::globals->frame_time;
	else
		prev_player_hp[entity->index()] = entity->health();

	auto health_color = color((255 - entity->health() * 2.55), (entity->health() * 2.55), 0, fade.at(entity->index()));
	
	if (entity->health() > 100)
		health_color = color::green(fade.at(entity->index()));

	render::filled_rect(_box.x - 6, _box.y - 1, 4, _box.h + 2, color::black(fade.at(entity->index()) / 1.5f));

	render::filled_rect(_box.x - 5, _box.y + (_box.h - _box.h * (prev_player_hp[entity->index()]) / 100.f), 2,
		_box.h * (prev_player_hp[entity->index()]) / 100.f - (entity->health() >= 100 ? 0 : -1), color(health_color));

	if (entity->health() < 100)
		render::text(_box.x - 4, _box.y - 1 + (_box.h - _box.h * (prev_player_hp[entity->index()]) / 100.f) - 3, render::fonts::name, std::to_string(static_cast<int>(prev_player_hp[entity->index()])), true, color::white(fade.at(entity->index())));
}

void visuals::player::weapon(player_t* entity, visuals::box _box) {
	auto weapon = entity->active_weapon();

	if (!weapon)
		return;

	auto weapon_data = weapon->get_weapon_data();

	if (!weapon_data)
		return;

	auto ammo_primary = weapon->clip1_count();
	auto ammo_secondary = weapon->primary_reserve_ammo_acount();

	if (configuration::settings::visuals_player_weapon) {
		auto localized_name = interfaces::localize->find(weapon_data->hud_name);

		std::wstring w(localized_name);

		if (w.empty())
			return;

		std::string ret(w.begin(), w.end());

		if (ret.empty())
			return; 

		std::transform(ret.begin(), ret.end(), ret.begin(), ::toupper);

		render::text(_box.x + (_box.w / 2), _box.y + _box.h + (!entity->armor() || !configuration::settings::visuals_player_armor_bar ? 1 : 6),
			render::fonts::name, ret, true, color(configuration::settings::visuals_weapon_color[0] * 255.f, configuration::settings::visuals_weapon_color[1] * 255.f, configuration::settings::visuals_weapon_color[2] * 255.f, fade.at(entity->index())));
	}

	if (configuration::settings::visuals_player_weapon) {
		if (!ammo_primary || !ammo_secondary)
			return;

		render::text(_box.x + (_box.w / 2), _box.y + _box.h + (!entity->armor() || !configuration::settings::visuals_player_armor_bar ? 8 : 13),
			render::fonts::name, std::to_string(ammo_primary).append("/").append(std::to_string(ammo_secondary)).c_str(), true,
			color(configuration::settings::visuals_weapon_color[0] * 255.f, configuration::settings::visuals_weapon_color[1] * 255.f, configuration::settings::visuals_weapon_color[2] * 255.f, fade.at(entity->index())));
	}
}

void visuals::player::armor(player_t* entity, visuals::box _box) {
	if (!configuration::settings::visuals_player_armor_bar)
		return;

	auto armor = entity->armor();

	if (!armor)
		return;
	
	render::filled_rect(_box.x - 1, _box.y + _box.h + 2, _box.w + 2, 4, color::black(fade.at(entity->index()) / 1.5f));
	render::filled_rect(_box.x, _box.y + _box.h + 3, (_box.w * entity->armor()) / 100, 2, color(0, 80, 255, fade.at(entity->index())));
}

void visuals::entities::run() {
	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return;

	if (!csgo::local_player)
		return;

	if (!configuration::settings::visuals_enable)
		return;

	if (!should_render)
		return;

	for (int i = 0; i < interfaces::entity_list->get_highest_index(); i++) {
		auto entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));

		if (!entity || entity->dormant() || entity == csgo::local_player)
			continue;

		bomb_overlay(entity);

		visuals::box _box;
		if (!calculate_player_box(entity, _box))
			continue;

		danger_zone(entity, _box);
		planted_bomb(entity, _box);
		projectiles(entity, _box);
		dropped(entity, _box);
	}
}

void visuals::entities::planted_bomb(player_t* entity, visuals::box _box) {
	if (!configuration::settings::visuals_bomb_timer)
		return;

	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return;

	if (entity->client_class()->class_id != class_ids::cplantedc4)
		return;

	if (entity->c4_defused() || !entity->c4_ticking())
		return;

	vec3_t origin = vec3_t(0, 0, 0), position = vec3_t(0, 0, 0);
	origin = entity->origin();

	if (!math::world_to_screen(origin, position))
		return;

	if (configuration::settings::planted_c4_type[0])
		render::text(_box.x + (_box.w / 2), _box.y + _box.h + 1, render::fonts::name, "BOMB", true, color(configuration::settings::visuals_bomb_timer_color[0] * 255,
			configuration::settings::visuals_bomb_timer_color[1] * 255, configuration::settings::visuals_bomb_timer_color[2] * 255, configuration::settings::visuals_bomb_timer_color[3] * 255));
	
	if (configuration::settings::planted_c4_type[1])
		render_entity_box(_box, color(configuration::settings::visuals_bomb_timer_color[0] * 255,
			configuration::settings::visuals_bomb_timer_color[1] * 255, configuration::settings::visuals_bomb_timer_color[2] * 255, configuration::settings::visuals_bomb_timer_color[3] * 255), (configuration::settings::visuals_bomb_timer_color[3] * 255));
}

void visuals::entities::bomb_overlay(player_t* entity) {
	if (!configuration::settings::visuals_bomb_timer || !configuration::settings::planted_c4_type[2])
		return;

	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return;

	if (entity->client_class()->class_id != class_ids::cplantedc4)
		return;

	if (entity->c4_defused() || !entity->c4_ticking())
		return;

	auto time_to_explode = (entity->c4_blow_time() - interfaces::globals->cur_time);

	if (time_to_explode <= 0.1f)
		return;

	int width, height;
	interfaces::engine->get_screen_size(width, height);

	static auto c4_timer = interfaces::console->get_convar("mp_c4timer")->get_int();
	auto value = (time_to_explode * width) / c4_timer;

	if (value <= 0.1f)
		return;

	render::filled_rect(0, 0, value, 10, color::green(150));
	render::text(value, 10, render::fonts::watermark, (std::stringstream{ } << std::setprecision(3) << (std::max)(time_to_explode, 0.0f) << " S.").str().c_str(), false, color::white());

	if (csgo::local_player->is_alive()) {
		const auto percentage = 1.0f;
		auto dmg = 500.f;
		auto radius = dmg * 3.5f;
		auto distance = (entity->get_eye_pos() - csgo::local_player->get_eye_pos()).length();
		auto sigma = radius / 3.0f;
		auto fall_off = exp(-distance * distance / (2.0f * sigma * sigma));
		auto final_dmg = dmg * fall_off * percentage;

		auto scale_damage = [&](float dmg, int armor_value) {
			float ratio = 0.5f, bonus = 0.5f;
			if (armor_value > 0) {
				float _new = dmg * ratio;
				float armor = (dmg - _new) * bonus;

				if (armor > static_cast<float>(armor_value)) {
					armor = static_cast<float>(armor_value)* (1.f / bonus);
					_new = dmg - armor;
				}

				dmg = _new;
			}
			return dmg;
		};

		final_dmg = scale_damage(final_dmg, csgo::local_player->armor());

		if (final_dmg > 1.f)
			render::text(value, 20, render::fonts::watermark, final_dmg > csgo::local_player->health() ? "FATAL" : (std::stringstream{ } << "-" << std::setprecision(3) << final_dmg << " DMG.").str().c_str(), false, final_dmg > csgo::local_player->health() ? color::red() : color::white());
	}

}

void visuals::entities::projectiles(player_t* entity, visuals::box _box) {
	if (!configuration::settings::visuals_projectiles)
		return;

	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return;

	auto client_class = entity->client_class();

	if (!client_class)
		return;

	std::string model_name = interfaces::model_info->get_model_name(entity->model());

	if (model_name.empty())
		return;

	vec3_t grenade_position, grenade_origin;
	grenade_origin = entity->origin();

	if (grenade_origin.is_zero())
		return;

	if (!math::world_to_screen(grenade_origin, grenade_position))
		return;
	
	if (client_class->class_id == class_ids::cbasecsgrenadeprojectile) {
		if (model_name.find("fraggrenade") != std::string::npos && entity->explode_effect_tick_begin() < 1) {
			if (configuration::settings::visuals_projectiles_type[1])
				render_entity_box(_box, color::red(255), 255);

			if (configuration::settings::visuals_projectiles_type[0])
				render::text(_box.x + (_box.w / 2), _box.y + _box.h + 1, render::fonts::name, ("GRENADE"), true, color::red(230));
		}

		else if (model_name.find("flashbang") != std::string::npos) {
			if (configuration::settings::visuals_projectiles_type[1])
				render_entity_box(_box, color::yellow(255), 255);

			if (configuration::settings::visuals_projectiles_type[0])
				render::text(_box.x + (_box.w / 2), _box.y + _box.h + 1, render::fonts::name, ("FLASHBANG"), true, color::yellow(230));
		}
	}

	if (client_class->class_id == class_ids::cmolotovprojectile) {
		if (model_name.find("incendiarygrenade") != std::string::npos) {
			if (configuration::settings::visuals_projectiles_type[1])
				render_entity_box(_box, color::red(255), 255);

			if (configuration::settings::visuals_projectiles_type[0])
				render::text(_box.x + (_box.w / 2), _box.y + _box.h + 1, render::fonts::name, ("INCENDIARY"), true, color::red(230));
		}

		else if (model_name.find("molotov") != std::string::npos) {
			if (configuration::settings::visuals_projectiles_type[1])
				render_entity_box(_box, color::red(255), 255);

			if (configuration::settings::visuals_projectiles_type[0])
				render::text(_box.x + (_box.w / 2), _box.y + _box.h + 1, render::fonts::name, ("MOLOTOV"), true, color::red(230));
		}
	}

	if (client_class->class_id == class_ids::csmokegrenadeprojectile) {
		if (model_name.find("smokegrenade") != std::string::npos) {
			if (configuration::settings::visuals_projectiles_type[1])
				render_entity_box(_box, color::yellow(255), 255);

			if (configuration::settings::visuals_projectiles_type[0])
				render::text(_box.x + (_box.w / 2), _box.y + _box.h + 1, render::fonts::name, ("SMOKE"), true, color::yellow(230));
		}
	}

	if (client_class->class_id == class_ids::cdecoyprojectile) {
		if (model_name.find("decoy") != std::string::npos) {
			if (configuration::settings::visuals_projectiles_type[1])
				render_entity_box(_box, color::white(255), 255);

			if (configuration::settings::visuals_projectiles_type[0])
				render::text(_box.x + (_box.w / 2), _box.y + _box.h + 1, render::fonts::name, ("DECOY"), true, color::white(230));
		}
	}

	if (client_class->class_id == class_ids::cinferno) {
		if (configuration::settings::visuals_projectiles_type[0])
			render::text(_box.x + (_box.w / 2), _box.y + _box.h + 1, render::fonts::name, ("FIRE"), true, color::red(230));
	}	
}

void visuals::entities::dropped(player_t* entity, visuals::box _box) {
	if (!configuration::settings::visuals_dropped_weapons)
		return;

	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return;

	auto client_class = entity->client_class();

	if (!client_class)
		return;

	vec3_t dropped_position, dropped_origin;
	dropped_origin = entity->origin();

	if (dropped_origin.is_zero())
		return;

	if (!math::world_to_screen(dropped_origin, dropped_position))
		return;

	if (strstr(client_class->network_name, ("CWeapon"))) {
		std::string converted = std::string(client_class->network_name).substr(7).c_str();
		std::transform(converted.begin(), converted.end(), converted.begin(), ::toupper);

		if (converted.empty())
			return;
		
		if (configuration::settings::visuals_dropped_weapons_type[1])
			render_entity_box(_box, color(configuration::settings::visuals_dropped_weapons_color[0] * 255,
				configuration::settings::visuals_dropped_weapons_color[1] * 255, configuration::settings::visuals_dropped_weapons_color[2] * 255, configuration::settings::visuals_dropped_weapons_color[3] * 255), (configuration::settings::visuals_dropped_weapons_color[3] * 255));

		if (configuration::settings::visuals_dropped_weapons_type[0])
			render::text(_box.x + (_box.w / 2), _box.y + _box.h + 1, render::fonts::name, converted, true, color(configuration::settings::visuals_dropped_weapons_color[0] * 255,
				configuration::settings::visuals_dropped_weapons_color[1] * 255, configuration::settings::visuals_dropped_weapons_color[2] * 255, configuration::settings::visuals_dropped_weapons_color[3] * 255));
	}

	if (client_class->class_id == class_ids::cak47) {
		if (configuration::settings::visuals_dropped_weapons_type[1])
			render_entity_box(_box, color(configuration::settings::visuals_dropped_weapons_color[0] * 255,
				configuration::settings::visuals_dropped_weapons_color[1] * 255, configuration::settings::visuals_dropped_weapons_color[2] * 255, configuration::settings::visuals_dropped_weapons_color[3] * 255), (configuration::settings::visuals_dropped_weapons_color[3] * 255));

		if (configuration::settings::visuals_dropped_weapons_type[0])
			render::text(_box.x + (_box.w / 2), _box.y + _box.h + 1, render::fonts::name, ("AK47"), true, color(configuration::settings::visuals_dropped_weapons_color[0] * 255,
				configuration::settings::visuals_dropped_weapons_color[1] * 255, configuration::settings::visuals_dropped_weapons_color[2] * 255, configuration::settings::visuals_dropped_weapons_color[3] * 255));
	}
	
	if (client_class->class_id == class_ids::cc4) {
		if (configuration::settings::visuals_dropped_weapons_type[1])
			render_entity_box(_box, color(configuration::settings::visuals_dropped_weapons_color[0] * 255,
				configuration::settings::visuals_dropped_weapons_color[1] * 255, configuration::settings::visuals_dropped_weapons_color[2] * 255, configuration::settings::visuals_dropped_weapons_color[3] * 255), (configuration::settings::visuals_dropped_weapons_color[3] * 255));
		
		if (configuration::settings::visuals_dropped_weapons_type[0])
			render::text(_box.x + (_box.w / 2), _box.y + _box.h + 1, render::fonts::name, ("C4"), true, color(configuration::settings::visuals_dropped_weapons_color[0] * 255,
				configuration::settings::visuals_dropped_weapons_color[1] * 255, configuration::settings::visuals_dropped_weapons_color[2] * 255, configuration::settings::visuals_dropped_weapons_color[3] * 255));
	}

	if (client_class->class_id == class_ids::cdeagle) {
		if (configuration::settings::visuals_dropped_weapons_type[1])
			render_entity_box(_box, color(configuration::settings::visuals_dropped_weapons_color[0] * 255,
				configuration::settings::visuals_dropped_weapons_color[1] * 255, configuration::settings::visuals_dropped_weapons_color[2] * 255, configuration::settings::visuals_dropped_weapons_color[3] * 255), (configuration::settings::visuals_dropped_weapons_color[3] * 255));

		if (configuration::settings::visuals_dropped_weapons_type[0])
			render::text(_box.x + (_box.w / 2), _box.y + _box.h + 1, render::fonts::name, ("DEAGLE"), true, color(configuration::settings::visuals_dropped_weapons_color[0] * 255,
				configuration::settings::visuals_dropped_weapons_color[1] * 255, configuration::settings::visuals_dropped_weapons_color[2] * 255, configuration::settings::visuals_dropped_weapons_color[3] * 255));
	}
}

void visuals::entities::danger_zone(player_t* entity, visuals::box _box) {
	if (!configuration::settings::visuals_danger_zone_enable)
		return;

	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return;

	static auto game_type = interfaces::console->get_convar("game_type");

	if (game_type->get_int() != 6)
		return;

	auto client_class = entity->client_class();

	if (!client_class)
		return;

	vec3_t world_position, world_origin;
	world_origin = entity->origin();

	if (entity->origin().distance_to(csgo::local_player->origin()) > configuration::settings::visuals_danger_zone_render_distance)
		return;

	if (!math::world_to_screen(world_origin, world_position))
		return;

	if (configuration::settings::visuals_danger_zone_items[1] && client_class->class_id == citemcash)
		render::text(_box.x + (_box.w / 2), _box.y + _box.h + 1, render::fonts::name, ("CASH"), true, color::green());

	if (configuration::settings::visuals_danger_zone_items[6] && client_class->class_id == citem_healthshot)
		render::text(_box.x + (_box.w / 2), _box.y + _box.h + 1, render::fonts::name, ("HEALTH SHOT"), true, color::red());

	if (configuration::settings::visuals_danger_zone_items[4] && client_class->class_id == cdrone)
		render::text(_box.x + (_box.w / 2), _box.y + _box.h + 1, render::fonts::name, ("DRONE"), true, color::white());

	if (configuration::settings::visuals_danger_zone_items[4] && client_class->class_id == cdronegun)
		render::text(_box.x + (_box.w / 2), _box.y + _box.h + 1, render::fonts::name, ("TURRET"), true, color::white());

	if (configuration::settings::visuals_danger_zone_items[7] && client_class->class_id == cbreachcharge)
		render::text(_box.x + (_box.w / 2), _box.y + _box.h + 1, render::fonts::name, ("BREACH CHARGE"), true, color::white());

	if (configuration::settings::visuals_danger_zone_items[5] && client_class->class_id == cmelee)
		render::text(_box.x + (_box.w / 2), _box.y + _box.h + 1, render::fonts::name, ("MELEE WEAPON"), true, color::red());

	std::string model_name = interfaces::model_info->get_model_name(entity->model());

	if (model_name.empty())
		return;

	if (configuration::settings::visuals_danger_zone_items[2] && model_name.find("case_pistol") != std::string::npos)
		render::text(_box.x + (_box.w / 2), _box.y + _box.h + 1, render::fonts::name, ("PISTOL CASE"), true, color::red());
	else if (configuration::settings::visuals_danger_zone_items[2] && model_name.find("case_light_weapon") != std::string::npos)
		render::text(_box.x + (_box.w / 2), _box.y + _box.h + 1, render::fonts::name, ("LIGHT CASE"), true, color::red());
	else if (configuration::settings::visuals_danger_zone_items[2] && model_name.find("case_heavy_weapon") != std::string::npos)
		render::text(_box.x + (_box.w / 2), _box.y + _box.h + 1, render::fonts::name, ("HEAVY CASE"), true, color::red());
	else if (configuration::settings::visuals_danger_zone_items[2] && model_name.find("case_explosive") != std::string::npos)
		render::text(_box.x + (_box.w / 2), _box.y + _box.h + 1, render::fonts::name, ("EXPLOSIVES CASE"), true, color::red());
	else if (configuration::settings::visuals_danger_zone_items[2] && model_name.find("case_tools") != std::string::npos)
		render::text(_box.x + (_box.w / 2), _box.y + _box.h + 1, render::fonts::name, ("TOOLS CASE"), true, color::red());
	else if (configuration::settings::visuals_danger_zone_items[2] && model_name.find("random") != std::string::npos)
		render::text(_box.x + (_box.w / 2), _box.y + _box.h + 1, render::fonts::name, ("AIRDROP"), true, color(252, 186, 3));
	else if (configuration::settings::visuals_danger_zone_items[3] && model_name.find("dz_armor_helmet") != std::string::npos)
		render::text(_box.x + (_box.w / 2), _box.y + _box.h + 1, render::fonts::name, ("FULL ARMOR"), true, color(212, 0, 255));
	else if (configuration::settings::visuals_danger_zone_items[3] && model_name.find("dz_helmet") != std::string::npos)
		render::text(_box.x + (_box.w / 2), _box.y + _box.h + 1, render::fonts::name, ("HELMET"), true, color(212, 0, 255));
	else if (configuration::settings::visuals_danger_zone_items[3] && model_name.find("dz_armor") != std::string::npos)
		render::text(_box.x + (_box.w / 2), _box.y + _box.h + 1, render::fonts::name, ("ARMOR"), true, color(212, 0, 255));
	else if (configuration::settings::visuals_danger_zone_items[7] && model_name.find("upgrade_tablet") != std::string::npos)
		render::text(_box.x + (_box.w / 2), _box.y + _box.h + 1, render::fonts::name, ("TABLET UPGRADE"), true, color::white());
	else if (configuration::settings::visuals_danger_zone_items[7] && model_name.find("briefcase") != std::string::npos)
		render::text(_box.x + (_box.w / 2), _box.y + _box.h + 1, render::fonts::name, ("CONTRACT"), true, color::white());
	else if (configuration::settings::visuals_danger_zone_items[7] && model_name.find("parachutepack") != std::string::npos)
		render::text(_box.x + (_box.w / 2), _box.y + _box.h + 1, render::fonts::name, ("PARACHUTE"), true, color(212, 0, 255));
	else if (configuration::settings::visuals_danger_zone_items[1] && model_name.find("dufflebag") != std::string::npos)
		render::text(_box.x + (_box.w / 2), _box.y + _box.h + 1, render::fonts::name, ("CASH DUFFLEBAG"), true, color::green());
	else if (configuration::settings::visuals_danger_zone_items[2] && model_name.find("ammobox") != std::string::npos)
		render::text(_box.x + (_box.w / 2), _box.y + _box.h + 1, render::fonts::name, ("AMMOBOX"), true, color::red());
	else if (configuration::settings::visuals_danger_zone_items[7] && model_name.find("jammer") != std::string::npos)
		render::text(_box.x + (_box.w / 2), _box.y + _box.h + 1, render::fonts::name, ("RADAR JAMMER"), true, color::white());

}

void visuals::glow::run() {
	if (!configuration::settings::visuals_glow_enable)
		return;

	if (!csgo::local_player)
		return;

	if (!visuals::should_render)
		return;

	for (int i = 0; i < interfaces::glow_manager->size; i++) {
		auto& glow = interfaces::glow_manager->objects[i];

		if (glow.unused())
			continue;

		auto entity = reinterpret_cast<player_t*>(glow.entity);

		if (!entity || entity->dormant())
			continue;

		if (entity->client_class()->class_id != class_ids::ccsplayer)
			continue;

		if (entity->team() == csgo::local_player->team())
			continue;

		glow.set(configuration::settings::visuals_glow_color[0], configuration::settings::visuals_glow_color[1],
			configuration::settings::visuals_glow_color[2], configuration::settings::visuals_glow_color[3]);
	}
}