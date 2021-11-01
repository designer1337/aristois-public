#include "interfaces.hpp"
#include "../utilities/csgo.hpp"

#define _interface(val, type, module_name, interface_name) val = interfaces::get_interface<type*>((module_name), (interface_name));
#define _interface_offset(val, type, ptr, index, add, name) val = **reinterpret_cast<type***>((*reinterpret_cast<uintptr_t**>(ptr))[index] + add); if (val) utilities::console::log(("[ debug ] custom interface %s found at 0x%p \n"), name, val);

bool interfaces::initialize() {
	_interface(client, i_base_client_dll, "client.dll", "VClient018");
	_interface(entity_list, i_client_entity_list, "client.dll", "VClientEntityList003");
	_interface(engine, iv_engine_client, "engine.dll", "VEngineClient014");
	_interface(panel, i_panel, "vgui2.dll", "VGUI_Panel009");
	_interface(surface, i_surface, "vguimatsurface.dll", "VGUI_Surface031");
	_interface(material_system, i_material_system, "materialsystem.dll", "VMaterialSystem080");
	_interface(model_info, iv_model_info, "engine.dll", "VModelInfoClient004");
	_interface(model_render, iv_model_render, "engine.dll", "VEngineModel016");
	_interface(render_view, i_render_view, "engine.dll", "VEngineRenderView014");
	_interface(console, i_console, "vstdlib.dll", "VEngineCvar007");
	_interface(localize, i_localize, "localize.dll", "Localize_001");
	_interface(event_manager, i_game_event_manager2, "engine.dll", "GAMEEVENTSMANAGER002");
	_interface(debug_overlay, iv_debug_overlay, "engine.dll", "VDebugOverlay004");
	_interface(inputsystem, i_inputsytem, "inputsystem.dll", "InputSystemVersion001");
	_interface(trace_ray, trace, "engine.dll", "EngineTraceClient004");
	_interface(game_movement, player_game_movement, "client.dll", "GameMovement001");
	_interface(prediction, player_prediction, "client.dll", "VClientPrediction001");
	_interface(surface_props_physics, physics_surface_props, "vphysics.dll", "VPhysicsSurfaceProps001");

	_interface_offset(client_mode, i_client_mode, client, 10, 0x5, "client_mode");
	_interface_offset(globals, i_globals, client, 0, 0x1B, "global_vars");
	_interface_offset(client_state, i_client_state, engine, 12, 0x10, "client_state");

	device = **(IDirect3DDevice9***)(utilities::pattern_scan("shaderapidx9.dll", sig_device) + 1);

	if (device)
		utilities::console::log(("[ debug ] custom interface IDirect3DDevice9 found at 0x%p \n"), device);

	input = *(i_input * *)(utilities::pattern_scan("client.dll", sig_input) + 1);

	if (input)
		utilities::console::log(("[ debug ] custom interface input found at 0x%p \n"), input);

	glow_manager = (glow_manager_t*)(*(uintptr_t*)(utilities::pattern_scan("client.dll", sig_glow_manager) + 3));

	if (glow_manager)
		utilities::console::log(("[ debug ] custom interface glow_manager found at 0x%p \n"), glow_manager);

	move_helper = **(player_move_helper * **)(utilities::pattern_scan("client.dll", sig_player_move_helper) + 2);

	if (move_helper)
		utilities::console::log(("[ debug ] custom interface move_helper found at 0x%p \n"), move_helper);

	weapon_system = *(i_weapon_system * *)(utilities::pattern_scan("client.dll", sig_weapon_data) + 2);

	if (weapon_system)
		utilities::console::log(("[ debug ] custom interface weapon_system found at 0x%p \n"), weapon_system);

	utilities::console::log(("[ debug ] [->] interfaces intialized! \n"));
	return true;
}

#undef _interface
#undef _interface_offset