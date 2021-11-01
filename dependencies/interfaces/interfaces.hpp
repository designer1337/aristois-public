#pragma once
#include <windows.h>
#include <cstdint>
#include <string>
#include <stdexcept>
#include <d3dx9.h>

#include "i_base_client_dll.hpp"
#include "i_client_entity_list.hpp"
#include "iv_engine_client.hpp"
#include "i_client_state.hpp"
#include "i_panel.hpp"
#include "i_surface.hpp"
#include "i_global_vars_base.hpp"
#include "i_material_system.hpp"
#include "iv_model_info.hpp"
#include "iv_model_render.hpp"
#include "iv_debug_overlay.hpp"
#include "i_console.hpp"
#include "i_localize.hpp"
#include "i_game_event_manager.hpp"
#include "i_input.hpp"
#include "i_input_system.hpp"
#include "i_trace.hpp"
#include "i_render_view.hpp"
#include "i_glow_manager.hpp"
#include "i_player_movement.hpp"
#include "i_weapon_system.hpp"
#include "i_physics_surface_props.hpp"

namespace interfaces {
	template<typename T>
	__forceinline T get_interface(const char* module_name, const char* interface_name) {
		auto module_handle = GetModuleHandleA(module_name);
		if (!module_handle)
			return nullptr;

		auto create_interface_fn = reinterpret_cast<T(*)(const char*, int*)>(GetProcAddress(module_handle, ("CreateInterface")));
		if (!create_interface_fn)
			return nullptr;

		auto result = reinterpret_cast<T>(create_interface_fn(interface_name, nullptr));

		if (!result)
			throw std::runtime_error((std::string("failed to initialize interface: ").append(interface_name)).c_str());

		utilities::console::log(("[ debug ] interface %s found at 0x%p \n"), interface_name, result);

		return result;
	}

	inline i_base_client_dll* client{};
	inline i_input* input{};
	inline i_client_entity_list*entity_list{};
	inline iv_engine_client* engine{};
	inline i_client_mode* client_mode{};
	inline i_client_state* client_state{};
	inline i_panel* panel{};
	inline i_surface* surface{};
	inline i_globals* globals{};
	inline i_material_system* material_system{};
	inline iv_model_info* model_info{};
	inline iv_model_render* model_render{};
	inline i_render_view* render_view{};
	inline i_console* console{};
	inline i_localize* localize{};
	inline i_game_event_manager2* event_manager{};
	inline i_inputsytem* inputsystem{};
	inline iv_debug_overlay* debug_overlay{};
	inline trace* trace_ray{};
	inline glow_manager_t* glow_manager{};
	inline player_game_movement* game_movement{};
	inline player_prediction* prediction{};
	inline player_move_helper*move_helper{};
	inline i_weapon_system* weapon_system{};
	inline physics_surface_props* surface_props_physics{};
	inline IDirect3DDevice9* device{};

	bool initialize();
}