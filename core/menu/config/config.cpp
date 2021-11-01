#include "config.hpp"

//todo macro for saving and loading instead of just config header
#define config_header "csgo_base config"

std::map<int, const char*> weapon_names = {
{ item_definition_indexes::WEAPON_AK47, "AK-47" }, { item_definition_indexes::WEAPON_AUG, "AUG" },
{ item_definition_indexes::WEAPON_AWP, "AWP" }, { item_definition_indexes::WEAPON_CZ75A, "CZ75 Auto" },
{ item_definition_indexes::WEAPON_DEAGLE, "Desert Eagle" }, { item_definition_indexes::WEAPON_ELITE, "Dual Berettas" },
{ item_definition_indexes::WEAPON_FAMAS, "FAMAS" }, { item_definition_indexes::WEAPON_FIVESEVEN, "Five-SeveN" },
{ item_definition_indexes::WEAPON_G3SG1, "G3SG1" }, { item_definition_indexes::WEAPON_GALILAR, "Galil AR" },
{ item_definition_indexes::WEAPON_GLOCK, "Glock-18" }, { item_definition_indexes::WEAPON_M249, "M249" },
{ item_definition_indexes::WEAPON_M4A1_SILENCER, "M4A1-S" }, { item_definition_indexes::WEAPON_M4A1, "M4A4" },
{ item_definition_indexes::WEAPON_MAC10, "MAC-10" }, { item_definition_indexes::WEAPON_MAG7, "MAG-7" },
{ item_definition_indexes::WEAPON_MP7, "MP7" }, { item_definition_indexes::WEAPON_MP5SD, "MP5" },
{ item_definition_indexes::WEAPON_MP9, "MP9" }, { item_definition_indexes::WEAPON_NEGEV, "Negev" },
{ item_definition_indexes::WEAPON_NOVA, "Nova" }, { item_definition_indexes::WEAPON_HKP2000, "P2000" },
{ item_definition_indexes::WEAPON_P250, "P250" }, { item_definition_indexes::WEAPON_P90, "P90" },
{ item_definition_indexes::WEAPON_BIZON, "PP-Bizon" }, { item_definition_indexes::WEAPON_REVOLVER, "R8 Revolver" },
{ item_definition_indexes::WEAPON_SAWEDOFF, "Sawed-Off" }, { item_definition_indexes::WEAPON_SCAR20, "SCAR-20" },
{ item_definition_indexes::WEAPON_SSG08, "SSG 08" }, { item_definition_indexes::WEAPON_SG556, "SG 553" },
{ item_definition_indexes::WEAPON_TEC9, "Tec-9" }, { item_definition_indexes::WEAPON_UMP45, "UMP-45" },
{ item_definition_indexes::WEAPON_USP_SILENCER, "USP-S" }, { item_definition_indexes::WEAPON_XM1014, "XM1014" },
};

void configuration::initialize() {
	directory_path = "C:/csgo_base/configs";
	if (!std::filesystem::exists(directory_path))
		std::filesystem::create_directories(directory_path);

	directory_path_lua = "C:/csgo_base/lua";
	if (!std::filesystem::exists(directory_path_lua))
		std::filesystem::create_directories(directory_path_lua);
}

void configuration::save(const std::string name) {
	const auto path = directory_path + "/" + name;
	std::ofstream out(path);

	if (!out.is_open())
		return;

	Json::Value save;

	for (auto& [key, val] : weapon_names) {
		save[config_header][("enabled")][val] = settings::aimbot[key].enabled;
		save[config_header][("fov")][val] = settings::aimbot[key].fov;
		save[config_header][("smooth")][val] = settings::aimbot[key].smooth;
		save[config_header][("rcs")][val] = settings::aimbot[key].rcs;

		save[config_header][("hitboxes_one")][val] = settings::aimbot[key].hitboxes[0];
		save[config_header][("hitboxes_two")][val] = settings::aimbot[key].hitboxes[1];
		save[config_header][("hitboxes_three")][val] = settings::aimbot[key].hitboxes[2];
		save[config_header][("hitboxes_four")][val] = settings::aimbot[key].hitboxes[3];

		save[config_header][("hitchance")][val] = settings::triggerbot[key].hitchance;
		save[config_header][("delay")][val] = settings::triggerbot[key].delay;

	}

	//gui
	save[config_header][("gui_color_r")][0] = settings::gui_menu_accent[0];
	save[config_header][("gui_color_g")][1] = settings::gui_menu_accent[1];
	save[config_header][("gui_color_b")][2] = settings::gui_menu_accent[2];

	//aimbot
	save[config_header][("triggerbot_enable")] = settings::triggerbot_enable;
	save[config_header][("aimbot_backtrack_enable")] = settings::aimbot_backtrack_enable;
	save[config_header][("aimbot_anti_hit_enable")] = settings::aimbot_anti_hit_enable;
	save[config_header][("aimbot_anti_hit_choked_packets")] = settings::aimbot_anti_hit_choked_packets;
	save[config_header][("aimbot_target_in_air")] = settings::aimbot_target_in_air;
	save[config_header][("aimbot_target_smoke")] = settings::aimbot_target_smoke;
	save[config_header][("aimbot_target_team")] = settings::aimbot_target_team;
	save[config_header][("aimbot_target_visible")] = settings::aimbot_target_visible;
	save[config_header][("aimbot_type")] = settings::aimbot_type;
	save[config_header][("aimbot_delay_after_kill")] = settings::aimbot_delay_after_kill;
	save[config_header][("aimbot_trigger_key")] = settings::aimbot_trigger_key;
	save[config_header][("aimbot_auto_pistol")] = settings::aimbot_auto_pistol;

	//visuals
	save[config_header][("visuals_flags_one")] = settings::visuals_flags[0];
	save[config_header][("visuals_flags_two")] = settings::visuals_flags[1];
	save[config_header][("visuals_flags_three")] = settings::visuals_flags[2];
	save[config_header][("visuals_flags_four")] = settings::visuals_flags[3];
	save[config_header][("visuals_flags_five")] = settings::visuals_flags[4];
	save[config_header][("visuals_flags_six")] = settings::visuals_flags[5];
	save[config_header][("visuals_flags_seven")] = settings::visuals_flags[6];

	save[config_header][("visuals_danger_zone_items_one")] = settings::visuals_danger_zone_items[0];
	save[config_header][("visuals_danger_zone_items_two")] = settings::visuals_danger_zone_items[1];
	save[config_header][("visuals_danger_zone_items_three")] = settings::visuals_danger_zone_items[2];
	save[config_header][("visuals_danger_zone_items_four")] = settings::visuals_danger_zone_items[3];
	save[config_header][("visuals_danger_zone_items_five")] = settings::visuals_danger_zone_items[4];
	save[config_header][("visuals_danger_zone_items_six")] = settings::visuals_danger_zone_items[5];
	save[config_header][("visuals_danger_zone_items_seven")] = settings::visuals_danger_zone_items[6];
	save[config_header][("visuals_danger_zone_items_eight")] = settings::visuals_danger_zone_items[7];


	save[config_header][("planted_c4_type_one")] = settings::planted_c4_type[0];
	save[config_header][("planted_c4_type_two")] = settings::planted_c4_type[1];
	save[config_header][("planted_c4_type_three")] = settings::planted_c4_type[2];

	save[config_header][("visuals_dropped_weapons_type_one")] = settings::visuals_dropped_weapons_type[0];
	save[config_header][("visuals_dropped_weapons_type_two")] = settings::visuals_dropped_weapons_type[1];

	save[config_header][("visuals_projectiles")] = settings::visuals_projectiles;
	save[config_header][("visuals_projectiles_type_one")] = settings::visuals_projectiles_type[0];
	save[config_header][("visuals_projectiles_type_two")] = settings::visuals_projectiles_type[1];

	save[config_header][("visuals_dropped_weapons")] = settings::visuals_dropped_weapons;

	save[config_header][("visuals_dropped_weapons_color_r")][0] = settings::visuals_dropped_weapons_color[0];
	save[config_header][("visuals_dropped_weapons_color_g")][1] = settings::visuals_dropped_weapons_color[1];
	save[config_header][("visuals_dropped_weapons_color_b")][2] = settings::visuals_dropped_weapons_color[2];
	save[config_header][("visuals_dropped_weapons_color_a")][3] = settings::visuals_dropped_weapons_color[3];

	save[config_header][("visuals_name_color_r")][0] = settings::visuals_name_color[0];
	save[config_header][("visuals_name_color_g")][1] = settings::visuals_name_color[1];
	save[config_header][("visuals_name_color_b")][2] = settings::visuals_name_color[2];
	save[config_header][("visuals_name_color_a")][3] = settings::visuals_name_color[3];

	save[config_header][("visuals_box_color_r")][0] = settings::visuals_box_color[0];
	save[config_header][("visuals_box_color_g")][1] = settings::visuals_box_color[1];
	save[config_header][("visuals_box_color_b")][2] = settings::visuals_box_color[2];
	save[config_header][("visuals_box_color_a")][3] = settings::visuals_box_color[3];

	save[config_header][("visuals_weapon_color_r")][0] = settings::visuals_weapon_color[0];
	save[config_header][("visuals_weapon_color_g")][1] = settings::visuals_weapon_color[1];
	save[config_header][("visuals_weapon_color_b")][2] = settings::visuals_weapon_color[2];
	save[config_header][("visuals_weapon_color_a")][3] = settings::visuals_weapon_color[3];

	save[config_header][("visuals_toggle_key")] = settings::visuals_toggle_key;
	save[config_header][("visuals_toggle_type")] = settings::visuals_toggle_type;
	save[config_header][("visuals_enable")] = settings::visuals_enable;
	save[config_header][("visuals_enable_team")] = settings::visuals_enable_team;
	save[config_header][("visuals_draw_anti_hit_lines")] = settings::visuals_draw_anti_hit_lines;
	save[config_header][("visuals_fade")] = settings::visuals_fade;
	save[config_header][("visuals_nightmode")] = settings::visuals_nightmode;
	save[config_header][("visuals_chams_enable")] = settings::visuals_chams_enable;
	save[config_header][("visuals_chams_hidden_enable")] = settings::visuals_chams_hidden_enable;
	save[config_header][("visuals_chams_health_based")] = settings::visuals_chams_health_based;
	save[config_header][("visuals_backtrack_chams")] = settings::visuals_backtrack_chams;
	save[config_header][("visuals_chams_players_second_material")] = settings::visuals_chams_players_second_material;
	save[config_header][("visuals_chams_players_second_material_type")] = settings::visuals_chams_players_second_material_type;
	save[config_header][("visuals_chams_players_second_material_hidden")] = settings::visuals_chams_players_second_material_hidden;

	save[config_header][("visuals_chams_players_second_color_r")][0] = settings::visuals_chams_players_second_color[0];
	save[config_header][("visuals_chams_players_second_color_g")][1] = settings::visuals_chams_players_second_color[1];
	save[config_header][("visuals_chams_players_second_color_b")][2] = settings::visuals_chams_players_second_color[2];
	save[config_header][("visuals_chams_players_second_color_a")][3] = settings::visuals_chams_players_second_color[3];
	save[config_header][("visuals_chams_players_second_material_type_hidden")] = settings::visuals_chams_players_second_material_type_hidden;

	save[config_header][("visuals_chams_players_second_color_hidden_r")][0] = settings::visuals_chams_players_second_color_hidden[0];
	save[config_header][("visuals_chams_players_second_color_hidden_g")][1] = settings::visuals_chams_players_second_color_hidden[1];
	save[config_header][("visuals_chams_players_second_color_hidden_b")][2] = settings::visuals_chams_players_second_color_hidden[2];
	save[config_header][("visuals_chams_players_second_color_hidden_a")][3] = settings::visuals_chams_players_second_color_hidden[3];

	save[config_header][("visuals_backtrack_chams_color_r")][0] = settings::visuals_backtrack_chams_color[0];
	save[config_header][("visuals_backtrack_chams_color_g")][1] = settings::visuals_backtrack_chams_color[1];
	save[config_header][("visuals_backtrack_chams_color_b")][2] = settings::visuals_backtrack_chams_color[2];
	save[config_header][("visuals_backtrack_chams_color_a")][3] = settings::visuals_backtrack_chams_color[3];

	save[config_header][("visuals_chams_color_r")][0] = settings::visuals_chams_color[0];
	save[config_header][("visuals_chams_color_g")][1] = settings::visuals_chams_color[1];
	save[config_header][("visuals_chams_color_b")][2] = settings::visuals_chams_color[2];
	save[config_header][("visuals_chams_color_a")][3] = settings::visuals_chams_color[3];

	save[config_header][("visuals_chams_color_hidden_r")][0] = settings::visuals_chams_color_hidden[0];
	save[config_header][("visuals_chams_color_hidden_g")][1] = settings::visuals_chams_color_hidden[1];
	save[config_header][("visuals_chams_color_hidden_b")][2] = settings::visuals_chams_color_hidden[2];
	save[config_header][("visuals_chams_color_hidden_a")][3] = settings::visuals_chams_color_hidden[3];

	save[config_header][("visuals_chams_type")] = settings::visuals_chams_type;
	save[config_header][("visuals_chams_type_hidden")] = settings::visuals_chams_type_hidden;
	save[config_header][("visuals_chams_hands_first_material")] = settings::visuals_chams_hands_first_material;
	save[config_header][("visuals_chams_hands_second_material")] = settings::visuals_chams_hands_second_material;
	save[config_header][("visuals_chams_hands_first_material_type")] = settings::visuals_chams_hands_first_material_type;
	save[config_header][("visuals_chams_hands_second_material_type")] = settings::visuals_chams_hands_second_material_type;
	save[config_header][("visuals_chams_hands_color_r")][0] = settings::visuals_chams_hands_color[0];
	save[config_header][("visuals_chams_hands_color_g")][1] = settings::visuals_chams_hands_color[1];
	save[config_header][("visuals_chams_hands_color_b")][2] = settings::visuals_chams_hands_color[2];
	save[config_header][("visuals_chams_hands_color_a")][3] = settings::visuals_chams_hands_color[3];

	save[config_header][("visuals_chams_hands_second_color_r")][0] = settings::visuals_chams_hands_second_color[0];
	save[config_header][("visuals_chams_hands_second_color_g")][1] = settings::visuals_chams_hands_second_color[1];
	save[config_header][("visuals_chams_hands_second_color_b")][2] = settings::visuals_chams_hands_second_color[2];
	save[config_header][("visuals_chams_hands_second_color_a")][3] = settings::visuals_chams_hands_second_color[3];


	save[config_header][("visuals_glow_enable")] = settings::visuals_glow_enable;
	save[config_header][("visuals_glow_color_r")][0] = settings::visuals_glow_color[0];
	save[config_header][("visuals_glow_color_g")][1] = settings::visuals_glow_color[1];
	save[config_header][("visuals_glow_color_b")][2] = settings::visuals_glow_color[2];
	save[config_header][("visuals_glow_color_a")][3] = settings::visuals_glow_color[3];

	save[config_header][("visuals_viewmodel_x")] = settings::visuals_viewmodel_x;
	save[config_header][("visuals_viewmodel_y")] = settings::visuals_viewmodel_y;
	save[config_header][("visuals_viewmodel_z")] = settings::visuals_viewmodel_z;

	save[config_header][("visuals_player_name")] = settings::visuals_player_name;
	save[config_header][("visuals_player_box")] = settings::visuals_player_box;
	save[config_header][("visuals_player_health")] = settings::visuals_player_health;
	save[config_header][("visuals_player_weapon")] = settings::visuals_player_weapon;
	save[config_header][("visuals_player_armor_bar")] = settings::visuals_player_armor_bar;
	save[config_header][("visuals_bomb_timer")] = settings::visuals_bomb_timer;
	save[config_header][("visuals_bomb_timer_color_r")][0] = settings::visuals_bomb_timer_color[0];
	save[config_header][("visuals_bomb_timer_color_g")][1] = settings::visuals_bomb_timer_color[1];
	save[config_header][("visuals_bomb_timer_color_b")][2] = settings::visuals_bomb_timer_color[2];
	save[config_header][("visuals_bomb_timer_color_a")][3] = settings::visuals_bomb_timer_color[3];
	save[config_header][("visuals_danger_zone_enable")] = settings::visuals_danger_zone_enable;
	save[config_header][("visuals_danger_zone_render_distance")] = settings::visuals_danger_zone_render_distance;
	save[config_header][("visuals_view_model_fov")] = settings::visuals_view_model_fov;
	save[config_header][("visuals_fov")] = settings::visuals_fov;
	save[config_header][("visuals_thirdperson_distance")] = settings::visuals_thirdperson_distance;
	save[config_header][("visuals_force_radar")] = settings::visuals_force_radar;

	//misc
	save[config_header][("movement_bunnyhop")] = settings::movement_bunnyhop;
	save[config_header][("movement_bunnyhop_hitchance")] = settings::movement_bunnyhop_hitchance;
	save[config_header][("misc_clantag_spammer")] = settings::misc_clantag_spammer;
	save[config_header][("misc_reveal_server_ranks")] = settings::misc_reveal_server_ranks;
	save[config_header][("misc_force_crosshair")] = settings::misc_force_crosshair;
	save[config_header][("misc_radio_spam")] = settings::misc_radio_spam;
	save[config_header][("misc_remove_flashbang")] = settings::misc_remove_flashbang;
	save[config_header][("misc_remove_flashbang_alpha")] = settings::misc_remove_flashbang_alpha;
	save[config_header][("misc_hitmarker")] = settings::misc_hitmarker;
	save[config_header][("misc_hitmarker_screen_effect")] = settings::misc_hitmarker_screen_effect;
	save[config_header][("misc_hitmarker_sound")] = settings::misc_hitmarker_sound;
	save[config_header][("misc_killspam")] = settings::misc_killspam;
	save[config_header][("misc_agent_changer_enable")] = settings::misc_agent_changer_enable;
	save[config_header][("misc_agent_ct")] = settings::misc_agent_ct;
	save[config_header][("misc_agent_t")] = settings::misc_agent_t;
	save[config_header][("misc_event_logger_one")] = settings::misc_event_logger[0];
	save[config_header][("misc_event_logger_two")] = settings::misc_event_logger[1];
	save[config_header][("misc_spectators")] = settings::misc_spectators;
	save[config_header][("misc_slide_walk")] = settings::misc_slide_walk;

	out << save;
	out.close();
}

void configuration::load(const std::string name) {
	const auto path = directory_path + "/" + name;
	std::ifstream in(path);

	if (!in.good())
		save(name);

	if (!in.is_open())
		return;

	Json::Value load;

	in >> load;

	for (auto& [key, val] : weapon_names) {
		settings::aimbot[key].enabled = load[config_header][("enabled")][val].asBool();
		settings::aimbot[key].fov = load[config_header][("fov")][val].asFloat();
		settings::aimbot[key].smooth = load[config_header][("smooth")][val].asFloat();
		settings::aimbot[key].rcs = load[config_header][("rcs")][val].asFloat();
		settings::aimbot[key].hitboxes[0] = load[config_header][("hitboxes_one")][val].asBool();
		settings::aimbot[key].hitboxes[1] = load[config_header][("hitboxes_two")][val].asBool();
		settings::aimbot[key].hitboxes[2] = load[config_header][("hitboxes_three")][val].asBool();
		settings::aimbot[key].hitboxes[3] = load[config_header][("hitboxes_four")][val].asBool();

		settings::triggerbot[key].hitchance = load[config_header][("hitchance")][val].asFloat();
		settings::triggerbot[key].delay = load[config_header][("delay")][val].asInt();
	}

	//gui
	settings::gui_menu_accent[0] = load[config_header][("gui_color_r")][0].asFloat();
	settings::gui_menu_accent[1] = load[config_header][("gui_color_g")][1].asFloat();
	settings::gui_menu_accent[2] = load[config_header][("gui_color_b")][2].asFloat();

	//aimbot
	settings::triggerbot_enable = load[config_header][("triggerbot_enable")].asBool();
	settings::aimbot_backtrack_enable = load[config_header][("aimbot_backtrack_enable")].asBool();
	settings::aimbot_anti_hit_enable = load[config_header][("aimbot_anti_hit_enable")].asBool();
	settings::aimbot_anti_hit_choked_packets = load[config_header][("aimbot_anti_hit_choked_packets")].asInt();
	settings::aimbot_target_in_air = load[config_header][("aimbot_target_in_air")].asBool();
	settings::aimbot_target_smoke = load[config_header][("aimbot_target_smoke")].asBool();
	settings::aimbot_target_team = load[config_header][("aimbot_target_team")].asBool();
	settings::aimbot_target_visible = load[config_header][("aimbot_target_visible")].asBool();
	settings::aimbot_type = load[config_header][("aimbot_type")].asInt();
	settings::aimbot_delay_after_kill = load[config_header][("aimbot_delay_after_kill")].asInt();
	settings::aimbot_trigger_key = load[config_header][("aimbot_trigger_key")].asInt();
	settings::aimbot_auto_pistol = load[config_header][("aimbot_auto_pistol")].asBool();

	//visuals
	settings::visuals_projectiles = load[config_header][("visuals_projectiles")].asBool();
	settings::planted_c4_type[0] = load[config_header][("planted_c4_type_one")].asBool();
	settings::planted_c4_type[1] = load[config_header][("planted_c4_type_two")].asBool();
	settings::planted_c4_type[2] = load[config_header][("planted_c4_type_three")].asBool();

	settings::visuals_dropped_weapons = load[config_header][("visuals_dropped_weapons")].asBool();
	settings::visuals_dropped_weapons_type[0] = load[config_header][("visuals_dropped_weapons_type_one")].asBool();
	settings::visuals_dropped_weapons_type[1] = load[config_header][("visuals_dropped_weapons_type_two")].asBool();

	settings::visuals_projectiles_type[0] = load[config_header][("visuals_projectiles_type_one")].asBool();
	settings::visuals_projectiles_type[1] = load[config_header][("visuals_projectiles_type_two")].asBool();

	settings::visuals_flags[0] = load[config_header][("visuals_flags_one")].asBool();
	settings::visuals_flags[1] = load[config_header][("visuals_flags_two")].asBool();
	settings::visuals_flags[2] = load[config_header][("visuals_flags_three")].asBool();
	settings::visuals_flags[3] = load[config_header][("visuals_flags_four")].asBool();
	settings::visuals_flags[4] = load[config_header][("visuals_flags_five")].asBool();
	settings::visuals_flags[5] = load[config_header][("visuals_flags_six")].asBool();
	settings::visuals_flags[6] = load[config_header][("visuals_flags_seven")].asBool();

	settings::visuals_danger_zone_items[0] = load[config_header][("visuals_danger_zone_items_one")].asBool();
	settings::visuals_danger_zone_items[1] = load[config_header][("visuals_danger_zone_items_two")].asBool();
	settings::visuals_danger_zone_items[2] = load[config_header][("visuals_danger_zone_items_three")].asBool();
	settings::visuals_danger_zone_items[3] = load[config_header][("visuals_danger_zone_items_four")].asBool();
	settings::visuals_danger_zone_items[4] = load[config_header][("visuals_danger_zone_items_five")].asBool();
	settings::visuals_danger_zone_items[5] = load[config_header][("visuals_danger_zone_items_six")].asBool();
	settings::visuals_danger_zone_items[6] = load[config_header][("visuals_danger_zone_items_seven")].asBool();
	settings::visuals_danger_zone_items[7] = load[config_header][("visuals_danger_zone_items_eight")].asBool();

	settings::visuals_toggle_key = load[config_header][("visuals_toggle_key")].asInt();
	settings::visuals_toggle_type = load[config_header][("visuals_toggle_type")].asInt();

	settings::visuals_enable = load[config_header][("visuals_enable")].asBool();
	settings::visuals_enable_team = load[config_header][("visuals_enable_team")].asBool();
	settings::visuals_fade = load[config_header][("visuals_fade")].asBool();
	settings::visuals_nightmode = load[config_header][("visuals_nightmode")].asBool();
	settings::visuals_draw_anti_hit_lines = load[config_header][("visuals_draw_anti_hit_lines")].asBool();
	settings::visuals_chams_enable = load[config_header][("visuals_chams_enable")].asBool();
	settings::visuals_chams_hidden_enable = load[config_header][("visuals_chams_hidden_enable")].asBool();
	settings::visuals_chams_health_based = load[config_header][("visuals_chams_health_based")].asBool();
	settings::visuals_backtrack_chams = load[config_header][("visuals_backtrack_chams")].asBool();
	settings::visuals_chams_players_second_material = load[config_header][("visuals_chams_players_second_material")].asBool();
	settings::visuals_chams_players_second_material_hidden = load[config_header][("visuals_chams_players_second_material_hidden")].asBool();
	settings::visuals_chams_hands_first_material = load[config_header][("visuals_chams_hands_first_material")].asBool();
	settings::visuals_chams_hands_second_material = load[config_header][("visuals_chams_hands_second_material")].asBool();
	settings::visuals_chams_hands_first_material_type = load[config_header][("visuals_chams_hands_first_material_type")].asInt();
	settings::visuals_chams_hands_second_material_type = load[config_header][("visuals_chams_hands_second_material_type")].asInt();

	settings::visuals_chams_hands_color[0] = load[config_header][("visuals_chams_hands_color_r")][0].asFloat();
	settings::visuals_chams_hands_color[1] = load[config_header][("visuals_chams_hands_color_g")][1].asFloat();
	settings::visuals_chams_hands_color[2] = load[config_header][("visuals_chams_hands_color_b")][2].asFloat();
	settings::visuals_chams_hands_color[3] = load[config_header][("visuals_chams_hands_color_a")][3].asFloat();
	
	settings::visuals_dropped_weapons_color[0] = load[config_header][("visuals_dropped_weapons_color_r")][0].asFloat();
	settings::visuals_dropped_weapons_color[1] = load[config_header][("visuals_dropped_weapons_color_g")][1].asFloat();
	settings::visuals_dropped_weapons_color[2] = load[config_header][("visuals_dropped_weapons_color_b")][2].asFloat();
	settings::visuals_dropped_weapons_color[3] = load[config_header][("visuals_dropped_weapons_color_a")][3].asFloat();

	settings::visuals_name_color[0] = load[config_header][("visuals_name_color_r")][0].asFloat();
	settings::visuals_name_color[1] = load[config_header][("visuals_name_color_g")][1].asFloat();
	settings::visuals_name_color[2] = load[config_header][("visuals_name_color_b")][2].asFloat();
	settings::visuals_name_color[3] = load[config_header][("visuals_name_color_a")][3].asFloat();

	settings::visuals_box_color[0] = load[config_header][("visuals_box_color_r")][0].asFloat();
	settings::visuals_box_color[1] = load[config_header][("visuals_box_color_g")][1].asFloat();
	settings::visuals_box_color[2] = load[config_header][("visuals_box_color_b")][2].asFloat();
	settings::visuals_box_color[3] = load[config_header][("visuals_box_color_a")][3].asFloat();

	settings::visuals_weapon_color[0] = load[config_header][("visuals_weapon_color_r")][0].asFloat();
	settings::visuals_weapon_color[1] = load[config_header][("visuals_weapon_color_g")][1].asFloat();
	settings::visuals_weapon_color[2] = load[config_header][("visuals_weapon_color_b")][2].asFloat();
	settings::visuals_weapon_color[3] = load[config_header][("visuals_weapon_color_a")][3].asFloat();

	settings::visuals_chams_hands_second_color[0] = load[config_header][("visuals_chams_hands_second_color_r")][0].asFloat();
	settings::visuals_chams_hands_second_color[1] = load[config_header][("visuals_chams_hands_second_color_g")][1].asFloat();
	settings::visuals_chams_hands_second_color[2] = load[config_header][("visuals_chams_hands_second_color_b")][2].asFloat();
	settings::visuals_chams_hands_second_color[3] = load[config_header][("visuals_chams_hands_second_color_a")][3].asFloat();


	settings::visuals_backtrack_chams_color[0] = load[config_header][("visuals_backtrack_chams_color_r")][0].asFloat();
	settings::visuals_backtrack_chams_color[1] = load[config_header][("visuals_backtrack_chams_color_g")][1].asFloat();
	settings::visuals_backtrack_chams_color[2] = load[config_header][("visuals_backtrack_chams_color_b")][2].asFloat();
	settings::visuals_backtrack_chams_color[3] = load[config_header][("visuals_backtrack_chams_color_a")][3].asFloat();

	settings::visuals_chams_color[0] = load[config_header][("visuals_chams_color_r")][0].asFloat();
	settings::visuals_chams_color[1] = load[config_header][("visuals_chams_color_g")][1].asFloat();
	settings::visuals_chams_color[2] = load[config_header][("visuals_chams_color_b")][2].asFloat();
	settings::visuals_chams_color[3] = load[config_header][("visuals_chams_color_a")][3].asFloat();
	settings::visuals_chams_type = load[config_header][("visuals_chams_type")].asInt();
	settings::visuals_chams_type_hidden = load[config_header][("visuals_chams_type_hidden")].asInt();

	settings::visuals_chams_players_second_material_type = load[config_header][("visuals_chams_players_second_material_type")].asInt();
	settings::visuals_chams_players_second_material_type_hidden = load[config_header][("visuals_chams_players_second_material_type_hidden")].asInt();

	settings::visuals_glow_enable = load[config_header][("visuals_glow_enable")].asBool();
	settings::visuals_glow_color[0] = load[config_header][("visuals_glow_color_r")][0].asFloat();
	settings::visuals_glow_color[1] = load[config_header][("visuals_glow_color_g")][1].asFloat();
	settings::visuals_glow_color[2] = load[config_header][("visuals_glow_color_b")][2].asFloat();
	settings::visuals_glow_color[3] = load[config_header][("visuals_glow_color_a")][3].asFloat();

	settings::visuals_chams_players_second_color_hidden[0] = load[config_header][("visuals_chams_players_second_color_hidden_r")][0].asFloat();
	settings::visuals_chams_players_second_color_hidden[1] = load[config_header][("visuals_chams_players_second_color_hidden_g")][1].asFloat();
	settings::visuals_chams_players_second_color_hidden[2] = load[config_header][("visuals_chams_players_second_color_hidden_b")][2].asFloat();
	settings::visuals_chams_players_second_color_hidden[3] = load[config_header][("visuals_chams_players_second_color_hidden_a")][3].asFloat();

	settings::visuals_chams_players_second_color[0] = load[config_header][("visuals_chams_players_second_color_r")][0].asFloat();
	settings::visuals_chams_players_second_color[1] = load[config_header][("visuals_chams_players_second_color_g")][1].asFloat();
	settings::visuals_chams_players_second_color[2] = load[config_header][("visuals_chams_players_second_color_b")][2].asFloat();
	settings::visuals_chams_players_second_color[3] = load[config_header][("visuals_chams_players_second_color_a")][3].asFloat();

	settings::visuals_chams_color_hidden[0] = load[config_header][("visuals_chams_color_hidden_r")][0].asFloat();
	settings::visuals_chams_color_hidden[1] = load[config_header][("visuals_chams_color_hidden_g")][1].asFloat();
	settings::visuals_chams_color_hidden[2] = load[config_header][("visuals_chams_color_hidden_b")][2].asFloat();
	settings::visuals_chams_color_hidden[3] = load[config_header][("visuals_chams_color_hidden_a")][3].asFloat();
	settings::visuals_viewmodel_x = load[config_header][("visuals_viewmodel_x")].asFloat();
	settings::visuals_viewmodel_y = load[config_header][("visuals_viewmodel_y")].asFloat();
	settings::visuals_viewmodel_z = load[config_header][("visuals_viewmodel_z")].asFloat();
	settings::visuals_player_name = load[config_header][("visuals_player_name")].asBool();
	settings::visuals_player_box = load[config_header][("visuals_player_box")].asBool();
	settings::visuals_player_health = load[config_header][("visuals_player_health")].asBool();
	settings::visuals_player_weapon = load[config_header][("visuals_player_weapon")].asBool();
	settings::visuals_player_armor_bar = load[config_header][("visuals_player_armor_bar")].asBool();
	settings::visuals_bomb_timer = load[config_header][("visuals_bomb_timer")].asBool();
	settings::visuals_bomb_timer_color[0] = load[config_header][("visuals_bomb_timer_color_r")][0].asFloat();
	settings::visuals_bomb_timer_color[1] = load[config_header][("visuals_bomb_timer_color_g")][1].asFloat();
	settings::visuals_bomb_timer_color[2] = load[config_header][("visuals_bomb_timer_color_b")][2].asFloat();
	settings::visuals_bomb_timer_color[3] = load[config_header][("visuals_bomb_timer_color_a")][3].asFloat();
	settings::visuals_danger_zone_enable = load[config_header][("visuals_danger_zone_enable")].asBool();
	settings::visuals_danger_zone_render_distance = load[config_header][("visuals_danger_zone_render_distance")].asInt();
	settings::visuals_view_model_fov = load[config_header][("visuals_view_model_fov")].asFloat();
	settings::visuals_fov = load[config_header][("visuals_fov")].asFloat();
	settings::visuals_thirdperson_distance = load[config_header][("visuals_thirdperson_distance")].asFloat();
	settings::visuals_force_radar = load[config_header][("visuals_force_radar")].asBool();

	//misc
	settings::movement_bunnyhop = load[config_header][("movement_bunnyhop")].asBool();
	settings::movement_bunnyhop_hitchance = load[config_header][("movement_bunnyhop_hitchance")].asInt();
	settings::misc_clantag_spammer = load[config_header][("misc_clantag_spammer")].asBool();
	settings::misc_reveal_server_ranks = load[config_header][("misc_reveal_server_ranks")].asBool();
	settings::misc_force_crosshair = load[config_header][("misc_force_crosshair")].asBool();
	settings::misc_radio_spam = load[config_header][("misc_radio_spam")].asBool();
	settings::misc_remove_flashbang = load[config_header][("misc_remove_flashbang")].asBool();
	settings::misc_remove_flashbang_alpha = load[config_header][("misc_remove_flashbang_alpha")].asFloat();
	settings::misc_hitmarker = load[config_header][("misc_hitmarker")].asBool();
	settings::misc_hitmarker_screen_effect = load[config_header][("misc_hitmarker_screen_effect")].asBool();
	settings::misc_hitmarker_sound = load[config_header][("misc_hitmarker_sound")].asBool();
	settings::misc_killspam = load[config_header][("misc_killspam")].asBool();
	settings::misc_agent_changer_enable = load[config_header][("misc_agent_changer_enable")].asBool();
	settings::misc_agent_ct = load[config_header][("misc_agent_ct")].asInt();
	settings::misc_agent_t = load[config_header][("misc_agent_t")].asInt();
	settings::misc_event_logger[0] = load[config_header][("misc_event_logger_one")].asBool();
	settings::misc_event_logger[1] = load[config_header][("misc_event_logger_two")].asBool();
	settings::misc_spectators = load[config_header][("misc_spectators")].asBool();
	settings::misc_slide_walk = load[config_header][("misc_slide_walk")].asBool();


	in.close();
}

