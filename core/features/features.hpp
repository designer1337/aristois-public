#pragma once
#include "../../dependencies/utilities/csgo.hpp"
#include "../menu/config/config.hpp"
#include "../features/misc/engine_prediction.hpp"

struct sequence_object_t {
	sequence_object_t(int iInReliableState, int iOutReliableState, int iSequenceNr, float flCurrentTime)
		: iInReliableState(iInReliableState), iOutReliableState(iOutReliableState), iSequenceNr(iSequenceNr), flCurrentTime(flCurrentTime) { }

	int iInReliableState;
	int iOutReliableState;
	int iSequenceNr;
	float flCurrentTime;
};

namespace lag_compensation {
	void update(int stage);
	void run(c_usercmd* cmd);
	void update_incoming_sequences(i_net_channel* net_channel);
	void clear_incoming_sequences();
	void add_latency_to_net_channel(i_net_channel* net_channel, float latency);

	inline std::deque<sequence_object_t> sequences_vector = { };
	inline int last_incoming_sequence = 0;

	struct stored_records {
		vec3_t hitbox;
		vec3_t origin;
		float simulation_time;
		matrix_t matrix[256];
	};

	struct convars {
		convar* get_updaterate;
		convar* get_maxupdaterate;
		convar* get_interp;
		convar* get_interpratio;
		convar* get_mininterpratio;
		convar* get_maxinterpratio;
		convar* get_maxunlag;
	};

	inline convars cvars;
	inline std::deque<stored_records> records[65];

	constexpr auto lerp_time() {
		auto ratio = std::clamp(cvars.get_interpratio->get_float(), cvars.get_mininterpratio->get_float(), cvars.get_maxinterpratio->get_float());

		return max(cvars.get_interp->get_float(), (ratio / ((cvars.get_maxupdaterate) ? cvars.get_maxupdaterate->get_float() : cvars.get_updaterate->get_float())));
	}
	
	inline auto valid(float simtime) {
		auto network = interfaces::engine->get_net_channel_info();
		if (!network)
			return false;

		auto delta = std::clamp(network->get_latency(0) + network->get_latency(1) + lerp_time(), 0.f, cvars.get_maxunlag->get_float()) - (csgo::server_time() - simtime);
		return std::fabsf(delta) <= 0.2f;
	}

	constexpr auto time_to_ticks(float time) {
		return static_cast<int>(0.5f + time / interfaces::globals->interval_per_tick);
	}

	static void init() {
		records->clear();

		cvars.get_updaterate = interfaces::console->get_convar("cl_updaterate");
		cvars.get_maxupdaterate = interfaces::console->get_convar("sv_maxupdaterate");
		cvars.get_interp = interfaces::console->get_convar("cl_interp");
		cvars.get_interpratio = interfaces::console->get_convar("cl_interp_ratio");
		cvars.get_mininterpratio = interfaces::console->get_convar("sv_client_min_interp_ratio");
		cvars.get_maxinterpratio = interfaces::console->get_convar("sv_client_max_interp_ratio");
		cvars.get_maxunlag = interfaces::console->get_convar("sv_maxunlag");
	}
}

namespace skins {
	namespace knife {
		void run(int stage);
	}

	namespace agents {
		void run(int stage);
	}
}

namespace legit_bot {
	void event(i_game_event* event);
	void aimbot(c_usercmd* cmd);
	bool hit_chance(vec3_t ang, player_t* player, float hitchance);
	void trigger(c_usercmd* cmd);

	inline int kill_delay = 0;
	inline int trigger_delay = 0;

	namespace autowall {
		float get_damage_multiplier(int hit_group);
		bool is_armored(int hit_group, bool helmet);
		bool trace_to_exit(vec3_t& end, trace_t& tr, float x, float y, float z, float x2, float y2, float z2, trace_t* trace);
		static float handle_bullet_penetration(surface_data* enter_surface_data, trace_t& enter_trace, const vec3_t& direction, vec3_t& result, float penetration, float damage);
		static bool is_able_to_scan(player_t* local_player, entity_t* entity, const vec3_t& destination, const weapon_info_t* weapon_data, int min_damage);
	}

}

namespace misc {
	inline bool once = false;
	inline std::string backup_local_player_name;
	inline static int counter = 0;
	inline int hitmarker_time = 0;
	inline float delta = 0.f;
	inline bool side = false;
	inline bool break_lby = false;
	inline float next_update = 0;
	inline bool thirdperson = false;

	namespace movement {
		void bunny_hop(c_usercmd* cmd);
		void slide_walk(c_usercmd* cmd);
		void edge_jump(c_usercmd* cmd);
	};

	namespace hitmarker {
		void draw();
		void event(i_game_event* event);
	}

	namespace anti_hit {
		int get_automatic_anti_hit_side(float yaw);
		void update_lowerbody_breaker();
		void run(c_usercmd* cmd, bool& send_packet);
		void draw_gui();
	}

	namespace logger {
		void events(i_game_event* event);
	}

	namespace notification_system {
		struct notify_t {
			std::string text;
			float time;
			color _color;
			notify_t(std::string _text, color __color) {
				text = _text;
				_color = __color;
				time = interfaces::globals->cur_time;
			}
		};

		inline std::vector<notify_t> notify_list;

		void draw();
		void notify(std::string text, color _color);
	}

	void correct_angles(int stage);
	void third_person();
	void correct_local_animations(int stage);
	void draw_spectators();
	void kill_messages(i_game_event* event);
	void remove_flash();
	void remove_grass_danger_zone();
	void clantag_spammer();
	void name_spammer();
	void reveal_server_ranks(c_usercmd* cmd);
	void sniper_force_crosshair();
	void view_model_offsets();
}

namespace visuals {

	struct box {
		int x, y, w, h;
		box() = default;
		box(int x, int y, int w, int h) {
			this->x = x;
			this->y = y;
			this->w = w;
			this->h = h;
		}
	};

	bool calculate_player_box(player_t* entity, visuals::box& in);
	void activation_type();

	namespace player {
		void run();
		void animate(player_t* entity, const int index);
		void skeleton(player_t* entity, color _color, matrix_t matrix[256]);
		void name(player_t* entity, visuals::box _box);
		void box(player_t* entity, visuals::box _box);
		void flags(player_t* entity, visuals::box _box);
		void health(player_t* entity, visuals::box _box);
		void weapon(player_t* entity, visuals::box _box);
		void armor(player_t* entity, visuals::box _box);
	}

	namespace entities {
		void run();
		void planted_bomb(player_t* entity, visuals::box _box);
		void bomb_overlay(player_t* entity);
		void projectiles(player_t* entity, visuals::box _box);
		void danger_zone(player_t* entity, visuals::box _box);
		void dropped(player_t* entity, visuals::box _box);
	}

	namespace glow {
		void run();
	}

	inline std::array<float, 64> fade;
	inline bool should_render = false;
}

namespace chams {
	void initialize();

	namespace hands {
		void run(i_material_render_context* ctx, const draw_model_state_t& state, const model_render_info_t& info, matrix_t* bone_to_world);
	}

	namespace players {
		void run(i_material_render_context* ctx, const draw_model_state_t& state, const model_render_info_t& info, matrix_t* bone_to_world);
	}

	namespace materials {
		static i_material* textured;
		static i_material* flat;
	}
}