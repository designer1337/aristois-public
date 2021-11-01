#pragma once
#include "../../dependencies/utilities/csgo.hpp"
#include "../features/features.hpp"
#include "../features/misc/engine_prediction.hpp"
#include "event_listener.hpp"
#include "..//menu/menu.hpp"
#include "../scripts/scripts.hpp"

HWND hooks::window;
WNDPROC hooks::wndproc_original = NULL;

hooks::create_move::fn create_move_original = nullptr;
hooks::paint_traverse::fn paint_traverse_original = nullptr;
hooks::view_model::fn view_model_original = nullptr;
hooks::sv_cheats_get_bool::fn sv_cheats_get_bool_original = nullptr;
hooks::draw_model_execute::fn hooks::draw_model_execute::original = nullptr;
hooks::frame_stage_notify::fn frame_stage_notify_original = nullptr;
hooks::override_view::fn override_view_original = nullptr;
hooks::do_post_screen_effects::fn do_post_screen_effects_original = nullptr;
hooks::list_leaves_in_box::fn list_leaves_in_box_original = nullptr;
hooks::get_color_modulation::fn get_color_modulation_original = nullptr;
hooks::is_using_static_prop_debug_modes::fn is_using_static_prop_debug_modes_original = nullptr;
hooks::present::fn present_original = nullptr;
hooks::reset::fn reset_original = nullptr;

bool hooks::initialize() {
	static auto color_modulation = utilities::pattern_scan("materialsystem.dll", sig_get_color_modulation);
	static auto prop_debug_modes = utilities::pattern_scan("engine.dll", sig_is_using_static_prop_debug_modes);

	auto present_target = reinterpret_cast<void*>(get_virtual(interfaces::device, present::index));
	auto reset_target = reinterpret_cast<void*>(get_virtual(interfaces::device, reset::index));
	auto create_move_target = reinterpret_cast<void*>(get_virtual(interfaces::client_mode, create_move::index));
	auto view_model_target = reinterpret_cast<void*>(get_virtual(interfaces::client_mode, view_model::index));
	auto override_view_target = reinterpret_cast<void*>(get_virtual(interfaces::client_mode, override_view::index));
	auto do_post_screen_effects_target = reinterpret_cast<void*>(get_virtual(interfaces::client_mode, do_post_screen_effects::index));
	auto paint_traverse_target = reinterpret_cast<void*>(get_virtual(interfaces::panel, paint_traverse::index));
	auto sv_cheats_get_bool_target = reinterpret_cast<void*>(get_virtual(interfaces::console->get_convar("sv_cheats"), sv_cheats_get_bool::index));
	auto draw_model_execute_target = reinterpret_cast<void*>(get_virtual(interfaces::model_render, draw_model_execute::index));
	auto frame_stage_notify_target = reinterpret_cast<void*>(get_virtual(interfaces::client, frame_stage_notify::index));
	auto list_leaves_in_box_target = reinterpret_cast<void*>(get_virtual(interfaces::engine->get_bsp_query(), list_leaves_in_box::index));
	auto get_color_modulation_target = reinterpret_cast<void*>(color_modulation);
	auto is_using_static_prop_debug_modes_target = reinterpret_cast<void*>(prop_debug_modes);
	
	if (MH_Initialize() != MH_OK) {
		throw std::runtime_error(("failed to initialize hooking system."));
		return false;
	}

	if (MH_CreateHook(present_target, &present::hook, reinterpret_cast<void**>(&present_original)) != MH_OK) {
		throw std::runtime_error(("failed to initialize present."));
		return false;
	}

	if (MH_CreateHook(reset_target, &reset::hook, reinterpret_cast<void**>(&reset_original)) != MH_OK) {
		throw std::runtime_error(("failed to initialize reset."));
		return false;
	}

	if (MH_CreateHook(create_move_target, &create_move::hook, reinterpret_cast<void**>(&create_move_original)) != MH_OK) {
		throw std::runtime_error(("failed to initialize create_move."));
		return false;
	}

	if (MH_CreateHook(paint_traverse_target, &paint_traverse::hook, reinterpret_cast<void**>(&paint_traverse_original)) != MH_OK) {
		throw std::runtime_error(("failed to initialize paint_traverse."));
		return false;
	}

	if (MH_CreateHook(view_model_target, &view_model::hook, reinterpret_cast<void**>(&view_model_original)) != MH_OK) {
		throw std::runtime_error(("failed to initialize view_model."));
		return false;
	}

	if (MH_CreateHook(sv_cheats_get_bool_target, &sv_cheats_get_bool::hook, reinterpret_cast<void**>(&sv_cheats_get_bool_original)) != MH_OK) {
		throw std::runtime_error(("failed to initialize sv_cheats."));
		return false;
	}

	if (MH_CreateHook(draw_model_execute_target, &draw_model_execute::hook, reinterpret_cast<void**>(&hooks::draw_model_execute::original)) != MH_OK) {
		throw std::runtime_error(("failed to initialize draw_model_execute."));
		return false;
	}

	if (MH_CreateHook(frame_stage_notify_target, &frame_stage_notify::hook, reinterpret_cast<void**>(&frame_stage_notify_original)) != MH_OK) {
		throw std::runtime_error(("failed to initialize frame_stage_notify."));
		return false;
	}

	if (MH_CreateHook(override_view_target, &override_view::hook, reinterpret_cast<void**>(&override_view_original)) != MH_OK) {
		throw std::runtime_error(("failed to initialize override_view."));
		return false;
	}

	if (MH_CreateHook(do_post_screen_effects_target, &hooks::do_post_screen_effects::hook, reinterpret_cast<void**>(&do_post_screen_effects_original)) != MH_OK) {
		throw std::runtime_error(("failed to initialize do_post_screen_effects."));
		return false;
	}

	if (MH_CreateHook(list_leaves_in_box_target, &hooks::list_leaves_in_box::hook, reinterpret_cast<void**>(&list_leaves_in_box_original)) != MH_OK) {
		throw std::runtime_error(("failed to initialize list_leaves_in_box."));
		return false;
	}

	if (MH_CreateHook(get_color_modulation_target, &hooks::get_color_modulation::hook, reinterpret_cast<void**>(&get_color_modulation_original)) != MH_OK) {
		throw std::runtime_error(("failed to initialize get_color_modulation."));
		return false;
	}

	if (MH_CreateHook(is_using_static_prop_debug_modes_target, &hooks::is_using_static_prop_debug_modes::hook, reinterpret_cast<void**>(&is_using_static_prop_debug_modes_original)) != MH_OK) {
		throw std::runtime_error(("failed to initialize is_using_static_prop_debug_modes."));
		return false;
	}
	
	if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK) {
		throw std::runtime_error(("failed to initialize hooks."));
		return false;
	}

	window = FindWindowW((L"Valve001"), NULL);
	if (!window)
		throw std::runtime_error(("failed to initialize game window."));

	wndproc_original = reinterpret_cast<WNDPROC>(SetWindowLongW(window, GWL_WNDPROC, reinterpret_cast<LONG>(wndproc)));

	utilities::console::log(("[ debug ] [->] hooks intialized! \n"));
	return true;
}

void hooks::release() {
	MH_Uninitialize();

	MH_DisableHook(MH_ALL_HOOKS);
	MH_RemoveHook(MH_ALL_HOOKS);

	SetWindowLongW(FindWindowW((L"Valve001"), NULL), GWL_WNDPROC, reinterpret_cast<LONG>(wndproc_original));
}

bool __fastcall hooks::create_move::hook(void* ecx, void* edx, int input_sample_frametime, c_usercmd* cmd) {
	create_move_original(input_sample_frametime, cmd);

	if (!cmd || !cmd->command_number)
		return create_move_original(input_sample_frametime, cmd);
	
	csgo::local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));

	uintptr_t* frame_pointer;
	__asm mov frame_pointer, ebp;
	bool& send_packet = *reinterpret_cast<bool*>(*frame_pointer - 0x1C);

	csgo::cmd = cmd;
	csgo::server_time(cmd);

	auto net_channel = interfaces::client_state->net_channel;
	auto old_viewangles = cmd->viewangles;
	auto old_forwardmove = cmd->forwardmove;
	auto old_sidemove = cmd->sidemove;

	misc::movement::bunny_hop(cmd);
	misc::clantag_spammer();
	misc::reveal_server_ranks(cmd);
	misc::sniper_force_crosshair();
	misc::remove_grass_danger_zone();
	misc::name_spammer();
	misc::movement::slide_walk(cmd);

	for (auto hk : lua::hook_manager::get_hooks("on_create_move")) {
		auto result = hk.func(cmd);
		if (!result.valid()) {
			sol::error err = result;
			interfaces::console->console_color_printf({ 255, 0, 0, 255 }, ("[lua] "));
			interfaces::console->console_printf(std::string(err.what()).append(" \n").c_str());
			utilities::console::log(err.what());
		}

	}

	prediction::start(cmd); {
		legit_bot::aimbot(cmd);
		legit_bot::trigger(cmd);
		misc::anti_hit::update_lowerbody_breaker();
		misc::anti_hit::run(cmd, send_packet);
		lag_compensation::run(cmd);
		//misc::movement::edge_jump(cmd);
	} prediction::end();

	math::correct_movement(old_viewangles, cmd, old_forwardmove, old_sidemove);

	if (send_packet)
		csgo::send_packet_angles = cmd->viewangles;

	if (configuration::settings::aimbot_backtrack_enable && configuration::settings::aimbot_backtrack_ping_spike)
		lag_compensation::update_incoming_sequences(net_channel);
	else
		lag_compensation::clear_incoming_sequences();
	
	if (net_channel != nullptr) {
		if (!send_datagram::send_datagram_detour.is_hooked())
			send_datagram::send_datagram_detour.create(reinterpret_cast<void*>(get_virtual(net_channel, 46)), hooks::send_datagram::hook);
	}


	csgo::send_packet = send_packet;

	cmd->forwardmove = std::clamp(cmd->forwardmove, -450.0f, 450.0f);
	cmd->sidemove = std::clamp(cmd->sidemove, -450.0f, 450.0f);
	cmd->upmove = std::clamp(cmd->upmove, -320.f, 320.f);

	cmd->viewangles.normalize();
	cmd->viewangles.x = std::clamp(cmd->viewangles.x, -89.0f, 89.0f);
	cmd->viewangles.y = std::clamp(cmd->viewangles.y, -180.0f, 180.0f);
	cmd->viewangles.z = 0.0f;

	return false;
}

int __fastcall hooks::send_datagram::hook(i_net_channel* net_channel, int edx, bf_write* datagram) {
	static auto original = hooks::send_datagram::send_datagram_detour.original<decltype(&send_datagram::hook)>();

	if (!interfaces::engine->is_in_game() || !configuration::settings::aimbot_backtrack_enable || !configuration::settings::aimbot_backtrack_ping_spike || datagram != nullptr)
		return original(net_channel, edx, datagram);

	int state = net_channel->in_reliable_state;
	int sequence = net_channel->in_sequence_nr;
	
	lag_compensation::add_latency_to_net_channel(net_channel, configuration::settings::aimbot_backtrack_ping_spike_value);

	int _return = original(net_channel, edx, datagram);
	
	net_channel->in_reliable_state = state;
	net_channel->in_sequence_nr = sequence;

	return _return;
}

void __stdcall hooks::frame_stage_notify::hook(int frame_stage) {
	static auto lag_compensation_init = (lag_compensation::init(), false);

	if (!interfaces::engine->is_in_game()) {
		lag_compensation::clear_incoming_sequences();
		return frame_stage_notify_original(interfaces::client, frame_stage);
	}

	if (interfaces::engine->is_in_game())
		lag_compensation::update(frame_stage);

	skins::knife::run(frame_stage);
	skins::agents::run(frame_stage);
	misc::correct_angles(frame_stage);
	misc::correct_local_animations(frame_stage);

	frame_stage_notify_original(interfaces::client, frame_stage);
}

void __stdcall hooks::draw_model_execute::hook(i_material_render_context* ctx, const draw_model_state_t& state, const model_render_info_t& info, matrix_t* bone_to_world) {
	if (interfaces::model_render->is_forced_material_override())
		return hooks::draw_model_execute::original(interfaces::model_render, ctx, state, info, bone_to_world);
	
	hooks::draw_model_execute::original(interfaces::model_render, ctx, state, info, bone_to_world);

	chams::hands::run(ctx, state, info, bone_to_world);
	chams::players::run(ctx, state, info, bone_to_world);
}

bool __fastcall hooks::sv_cheats_get_bool::hook(PVOID convar, int edx) {
	static auto cam_think = utilities::pattern_scan("client.dll", sig_cam_think);

	if (!convar)
		return false;

	if ((_ReturnAddress()) == cam_think && csgo::local_player && csgo::local_player->is_alive())
		return true;
	else
		return sv_cheats_get_bool_original(convar);
}

void __fastcall hooks::override_view::hook(void* _this, void* _edx, view_setup_t* setup) {
	if (csgo::local_player && csgo::local_player->is_alive() && !csgo::local_player->is_scoped() && !interfaces::input->in_thirdperson)
		setup->fov += configuration::settings::visuals_fov;

	misc::view_model_offsets();

	override_view_original(interfaces::client_mode, _this, setup);
}

int __stdcall hooks::do_post_screen_effects::hook(int value) {
	visuals::glow::run();
	misc::third_person();
	misc::remove_flash();

	return do_post_screen_effects_original(interfaces::client_mode, value);
}

float __fastcall hooks::view_model::hook() {
	if (csgo::local_player && csgo::local_player->is_alive())
		return view_model_original() + configuration::settings::visuals_view_model_fov;
	else
		return view_model_original();
}

void __stdcall hooks::paint_traverse::hook(unsigned int panel, bool force_repaint, bool allow_force) {
	auto panel_to_draw = utilities::fnv::hash(interfaces::panel->get_panel_name(panel));

	switch (panel_to_draw) {
	case utilities::fnv::hash("MatSystemTopPanel"):
		visuals::player::run();
		visuals::entities::run();
		misc::hitmarker::draw();
		misc::notification_system::draw();

		for (auto hk : lua::hook_manager::get_hooks("on_paint")) {
			auto result = hk.func();
			if (!result.valid()) {
				sol::error err = result;
				interfaces::console->console_color_printf({ 255, 0, 0, 255 }, ("[lua] "));
				interfaces::console->console_printf(std::string(err.what()).append(" \n").c_str());
				utilities::console::log(err.what());
			}

		}
		break;
	case utilities::fnv::hash("FocusOverlayPanel"):
		interfaces::panel->set_keyboard_input_enabled(panel, menu::settings::open);
		interfaces::panel->set_mouse_input_enabled(panel, menu::settings::open);
		break;
	}

	paint_traverse_original(interfaces::panel, panel, force_repaint, allow_force);
}

LRESULT __stdcall hooks::wndproc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
	if (message == WM_KEYDOWN && LOWORD(wparam) == VK_INSERT)
		menu::settings::open = !menu::settings::open;

	LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	ImGui_ImplWin32_WndProcHandler(window, message, wparam, lparam);

	return CallWindowProcW(wndproc_original, hwnd, message, wparam, lparam);
}

std::once_flag flag;
long __stdcall hooks::present::hook(IDirect3DDevice9* device, RECT* source_rect, RECT* dest_rect, HWND dest_window_override, RGNDATA* dirty_region) {
	std::call_once(flag, [&] { menu::initialize(); });
	
	static bool init_imgui{ ImGui_ImplDX9_Init(device) };

	device->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE);
	IDirect3DVertexDeclaration9* vertexDeclaration;
	device->GetVertexDeclaration(&vertexDeclaration);

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	menu::render();
	misc::anti_hit::draw_gui();
	misc::draw_spectators();

	if (!menu::settings::open)
		menu::settings::alpha = 0;

	ImGui::EndFrame();
	ImGui::Render();    
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	device->SetVertexDeclaration(vertexDeclaration);
	vertexDeclaration->Release();

	return present_original(device, source_rect, dest_rect, dest_window_override, dirty_region);
}

long __stdcall hooks::reset::hook(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* present_parameters) {
	ImGui_ImplDX9_InvalidateDeviceObjects();

	auto result = reset_original(device, present_parameters);

	ImGui_ImplDX9_CreateDeviceObjects();

	return result;
}

#define MAX_COORD_FLOAT ( 16384.0f )
#define MIN_COORD_FLOAT ( -MAX_COORD_FLOAT )
int __fastcall hooks::list_leaves_in_box::hook(void* bsp, void* edx, const vec3_t& mins, const vec3_t& maxs, unsigned short* list, int list_max) {
	static auto list_leaves = utilities::pattern_scan("client.dll", sig_list_leaves_in_box) + 5;
	auto info = *reinterpret_cast<renderable_info_t * *>(reinterpret_cast<uintptr_t>(_AddressOfReturnAddress()) + 0x14);

	if ((_ReturnAddress()) != list_leaves)
		return list_leaves_in_box_original(bsp, mins, maxs, list, list_max);

	if (!info || !info->renderable)
		return list_leaves_in_box_original(bsp, mins, maxs, list, list_max);

	auto entity = utilities::call_virtual_method<entity_t*>(info->renderable - 4, 7);

	if (!entity || entity->client_class()->class_id != class_ids::ccsplayer) //is_player
		return list_leaves_in_box_original(bsp, mins, maxs, list, list_max);

	info->flags &= ~0x100;
	info->flags2 |= 0x40;

	static const vec3_t map_min = vec3_t(MIN_COORD_FLOAT, MIN_COORD_FLOAT, MIN_COORD_FLOAT);
	static const vec3_t map_max = vec3_t(MAX_COORD_FLOAT, MAX_COORD_FLOAT, MAX_COORD_FLOAT);
	return list_leaves_in_box_original(bsp, map_min, map_max, list, list_max);
}

void __fastcall hooks::get_color_modulation::hook(void* ecx, void* edx, float* r, float* g, float* b) {
	get_color_modulation_original(ecx, r, g, b);

	if (!configuration::settings::visuals_nightmode)
		return get_color_modulation_original(ecx, r, g, b);

	const auto material = reinterpret_cast<i_material*>(ecx);

	if (!material || material->is_error_material())
		return get_color_modulation_original(ecx, r, g, b);

	const auto group = utilities::fnv::hash(material->get_texture_group_name());

	if (group != utilities::fnv::hash(("World textures")) && group != utilities::fnv::hash(("StaticProp textures"))
		&& (group != utilities::fnv::hash(("SkyBox textures"))))
		return get_color_modulation_original(ecx, r, g, b);

	bool is_prop = (group == utilities::fnv::hash(("StaticProp textures")));

	*r *= is_prop ? 0.45f : 0.15f;
	*g *= is_prop ? 0.45f : 0.15f;
	*b *= is_prop ? 0.45f : 0.15f;
}

bool __stdcall hooks::is_using_static_prop_debug_modes::hook() {
	return configuration::settings::visuals_nightmode;
}
