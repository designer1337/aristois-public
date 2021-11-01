#pragma once
#include <windows.h>
#include <iostream>
#include <cstdint>
#include <memory>
#include <vector>
#include <thread>
#include <chrono>
#include <array>
#include <fstream>
#include <istream>
#include <unordered_map>
#include <intrin.h>
#include <filesystem>

#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "freetype.lib")
#pragma comment(lib, "Lua-5.3.5.lib")
#include "../../dependencies/utilities/imgui/imgui.h"
#include "../../dependencies/utilities/imgui/imgui_impl_dx9.h"
#include "../../dependencies/utilities/imgui/imgui_impl_win32.h"

#include "../utilities/utilities.hpp"
#include "../utilities/minhook/minhook.h"
#include "../interfaces/interfaces.hpp"
#include "../../source-sdk/math/vectors.hpp"
#include "../../source-sdk/structs/color.hpp"
#include "../../source-sdk/classes/user_cmd.hpp"
#include "../../source-sdk/classes/recv_props.hpp"
#include "../../source-sdk/classes/client_class.hpp"
#include "../../source-sdk/classes/convar.hpp"
#include "../../source-sdk/classes/studio.hpp"
#include "../../source-sdk/structs/weaponinfo.hpp"
#include "../../source-sdk/classes/view_setup.hpp"
#include "../../source-sdk/classes/entities.hpp"
#include "../../source-sdk/structs/animstate.hpp"
#include "../../core/hooks/hooks.hpp"
#include "../../dependencies/math/math.hpp"
#include "../../dependencies/utilities/renderer/renderer.hpp"
#include "../utilities/csgo.hpp"
#include "../../dependencies/utilities/utilities.hpp"

//signatures updated 18.06.2020 - designer

//interfaces
#define sig_client_state "A1 ? ? ? ? 8B 80 ? ? ? ? C3"
#define sig_directx "A1 ? ? ? ? 50 8B 08 FF 51 0C"
#define sig_input "B9 ? ? ? ? F3 0F 11 04 24 FF 50 10"
#define sig_glow_manager "0F 11 05 ? ? ? ? 83 C8 01 C7 05 ? ? ? ? 00 00 00 00"
#define sig_player_move_helper "8B 0D ? ? ? ? 8B 46 08 68"
#define sig_weapon_data "8B 35 ? ? ? ? FF 10 0F B7 C0"
#define sig_device "A1 ? ? ? ? 50 8B 08 FF 51 0C"

//misc
#define sig_prediction_random_seed "8B 0D ? ? ? ? BA ? ? ? ? E8 ? ? ? ? 83 C4 04"
#define sig_line_goes_thru_smoke "55 8B EC 83 EC 08 8B 15 ? ? ? ? 0F 57 C0"
#define sig_apply_clan_tag "53 56 57 8B DA 8B F9 FF 15"
#define sig_cam_think "85 C0 75 30 38 86"
#define sig_get_color_modulation "55 8B EC 83 EC ? 56 8B F1 8A 46"
#define sig_is_using_static_prop_debug_modes "8B 0D ? ? ? ? 81 F9 ? ? ? ? 75 ? A1 ? ? ? ? 35 ? ? ? ? EB ? 8B 01 FF 50 ? 83 F8 ? 0F 85 ? ? ? ? 8B 0D"
#define sig_list_leaves_in_box "56 52 FF 50 18"
#define sig_trace_to_exit "55 8B EC 83 EC 30 F3 0F 10 75"

namespace csgo {
	extern player_t* local_player;
	extern c_usercmd* cmd;
	extern bool send_packet;
	float server_time(c_usercmd* = nullptr);
	inline float real_angle = 0.f;
	inline vec3_t send_packet_angles;
}