#pragma once
#include "../../../dependencies/utilities/csgo.hpp"
#include "../../../dependencies/utilities/json/json.hpp"

struct legitbot_settings {
	bool enabled = false;
	int aim_type = 1;
	float fov = 1.f;
	float rcs = 0.f;
	float smooth = 1.f;
	bool hitboxes[4] = { true, false, false, false };
};

struct triggerbot_settings {
	float hitchance = 1.f;
	int delay = 1;
};

namespace configuration {
	void initialize();
	void load(const std::string name);
	void save(const std::string name);

	inline std::string directory_path;
	inline std::string directory_path_lua;

	namespace settings {
		inline std::map<int, legitbot_settings> aimbot = { };
		inline std::map<int, triggerbot_settings> triggerbot = { };

		inline float gui_menu_accent[4]{ 0.37f, 0.41f, 0.75f, 1.0f };

		inline bool aimbot_backtrack_enable = false;
		inline bool aimbot_target_smoke = false;
		inline bool triggerbot_enable = false;
		inline bool aimbot_target_in_air = false;
		inline bool aimbot_target_team = false;
		inline bool aimbot_target_visible = false;
		inline int aimbot_trigger_key = 0;
		inline int aimbot_type = 0;
		inline int aimbot_trigger_type = 0;
		inline int aimbot_delay_after_kill = 0;
		inline float aimbot_auto_wall_mim_damage = 10.f;
		inline bool aimbot_auto_pistol = false;
		inline bool aimbot_backtrack_ping_spike = false;
		inline float aimbot_backtrack_ping_spike_value = 0.1f;

		inline bool aimbot_anti_hit_enable = false;
		inline int aimbot_anti_hit_choked_packets = 1;

		inline int visuals_toggle_type = 0;
		inline int visuals_toggle_key = 0;
		inline bool visuals_flags[7] = { false, false, false, false, false, false, false };
		inline 	bool planted_c4_type[3] = { false, false, false };
		inline 	bool visuals_projectiles = false;
		inline 	bool visuals_projectiles_type[2] = { false, false };
		inline 	bool visuals_enable = false;
		inline 	bool visuals_fade = false;
		inline 	bool visuals_nightmode = false;
		inline 	bool visuals_chams_enable = false;
		inline 	bool visuals_chams_hidden_enable = false;
		inline 	bool visuals_chams_players_second_material_hidden = false;
		inline 	bool visuals_chams_health_based = false;
		inline 	float visuals_chams_color[4]{ 33 / 255.f, 100 / 255.f , 207 / 255.f, 1.0f };
		inline 	bool visuals_backtrack_chams = false;
		inline 	float visuals_backtrack_chams_color[4]{ 33 / 255.f, 100 / 255.f , 207 / 255.f, 1.0f };
		inline 	bool visuals_chams_hands = false;
		inline 	float visuals_chams_hands_color[4]{ 33 / 255.f, 100 / 255.f , 207 / 255.f, 1.0f };
		inline 	int visuals_chams_type = 0;
		inline 	int visuals_chams_type_hidden = 0;
		inline 	int visuals_chams_hands_first_material_type = 0;
		inline 	int visuals_chams_hands_second_material_type = 0;
		inline 	int visuals_chams_players_second_material_type = 0;
		inline 	int visuals_chams_players_second_material_type_hidden = 0;
		inline 	bool visuals_chams_players_second_material = false;
		inline 	bool visuals_chams_hands_first_material = false;
		inline 	bool visuals_chams_hands_second_material = false;
		inline 	float visuals_chams_color_hidden[4]{ 255 / 255.f, 255 / 255.f , 255 / 255.f, 1.0f };
		inline 	int visuals_danger_zone_render_distance = 2500;
		inline 	bool visuals_danger_zone_items[8] = { false, false, false, false, false, false, false };;
		inline 	bool visuals_danger_zone_enable = false;
		inline 	bool visuals_enable_team = false;
		inline 	bool visuals_player_skeleton = false;
		inline 	float visuals_name_color[4]{ 255 / 255.f, 255 / 255.f , 255 / 255.f, 1.0f };
		inline 	float visuals_box_color[4]{ 255 / 255.f, 255 / 255.f , 255 / 255.f, 1.0f };
		inline 	float visuals_weapon_color[4]{ 255 / 255.f, 255 / 255.f , 255 / 255.f, 1.0f };

		inline 	float visuals_viewmodel_x = 3.f;
		inline 	float visuals_viewmodel_y = 3.f;
		inline 	float visuals_viewmodel_z = -3.f;

		inline 	bool visuals_player_name = false;
		inline 	bool visuals_player_weapon = false;
		inline 	bool visuals_player_armor_bar = false;
		inline 	bool visuals_player_box = false;
		inline 	bool visuals_player_health = false;
		inline 	float visuals_view_model_fov = 0.f;
		inline 	float visuals_fov = 0.f;
		inline 	float visuals_thirdperson_distance = 150.f;
		inline 	bool visuals_glow_enable = false;
		inline 	float visuals_glow_color[4]{ 0.7f, 0.9f, 0.4f, 1.f };
		inline 	bool visuals_bomb_timer = false;
		inline 	bool visuals_dropped_weapons = false;
		inline 	bool visuals_dropped_weapons_type[2] = { false, false };
		inline 	float visuals_dropped_weapons_color[4]{ 1.0f, 1.0f, 1.0f, 1.0f };
		inline 	float visuals_bomb_timer_color[4]{ 1.0f, 1.0f, 1.0f, 1.0f };
		inline float visuals_chams_hands_second_color[4]{ 1.0f, 1.0f, 1.0f, 1.0f };
		inline float visuals_chams_players_second_color[4]{ 1.0f, 1.0f, 1.0f, 1.0f };
		inline float visuals_chams_players_second_color_hidden[4]{ 1.0f, 1.0f, 1.0f, 1.0f };
		inline bool visuals_draw_anti_hit_lines = false;
		inline 	bool movement_bunnyhop = false;
		inline int movement_bunnyhop_hitchance = 100;
		inline bool visuals_force_radar = false;
		inline bool misc_name_spammer = false;

		inline 	bool misc_clantag_spammer = false;
		inline 	bool misc_reveal_server_ranks = false;
		inline 	bool misc_force_crosshair = false;
		inline 	bool misc_radio_spam = false;
		inline 	bool misc_remove_flashbang = false;
		inline 	float misc_remove_flashbang_alpha = 0.0f;
		inline bool misc_hitmarker = false;
		inline 	bool misc_hitmarker_sound = false;
		inline 	bool misc_hitmarker_screen_effect = false;
		inline 	bool misc_killspam = false;
		inline bool misc_event_logger[2] = { false, false };
		inline char misc_kill_message[256] = "rychu peja hedszoty strzela :D";
		inline 	char misc_clantag_text[256] = "aristois.me";
		inline 	bool misc_synced_clantag = false;
		inline 	bool misc_agent_changer_enable = false;
		inline 	int misc_agent_ct = 0;
		inline 	int misc_agent_t = 0;
		inline 	bool misc_slide_walk = false;
		inline 	bool misc_spectators = false;
		inline bool visuals_view_model_changer_enable = false;
		inline 	bool misc_matchmaking_region_enable = 0;
		inline 	int misc_matchmaking_region = 0;
	};
};