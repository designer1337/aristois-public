#include "menu.hpp"
#include "../../dependencies/utilities/imgui/imgui.h"
#include "../menu/config/config.hpp"
#include "../features/features.hpp"
#include "../../dependencies/utilities/imgui/freetype/imgui_freetype.h"
#include "../scripts/scripts.hpp"

#define GROUPBOX_FIRST_POSITION 14, 133
#define GROUPBOX_SECOND_POSITION 338,133
#define GROUPBOX_SIZE 308, 359

auto update_animation_alpha = [&]() {
    auto& style = ImGui::GetStyle();
    menu::settings::alpha = 0.f;
    menu::settings::alpha = std::clamp(menu::settings::alpha + menu::settings::animation_frequency * ImGui::GetIO().DeltaTime, 0.f, 1.f);
    style.Alpha = menu::settings::alpha;
};

auto tool_tip = [&](const std::string& string) {
    if (ImGui::IsItemHovered())
        menu::widgets::tool_tips.emplace_back(string);
};

std::vector<menu::settings::weapon_name_t> weapon_names = {
    { item_definition_indexes::WEAPON_AK47, "ak-47" },
    { item_definition_indexes::WEAPON_AUG, "aug" },
    { item_definition_indexes::WEAPON_AWP, "awp" },
    { item_definition_indexes::WEAPON_CZ75A, "cz75 auto" },
    { item_definition_indexes::WEAPON_DEAGLE, "desert eagle" },
    { item_definition_indexes::WEAPON_ELITE, "dual berettas" },
    { item_definition_indexes::WEAPON_FAMAS, "famas" },
    { item_definition_indexes::WEAPON_FIVESEVEN, "five-seveN" },
    { item_definition_indexes::WEAPON_G3SG1, "g3sg1" },
    { item_definition_indexes::WEAPON_GALILAR, "galil ar" },
    { item_definition_indexes::WEAPON_GLOCK, "glock-18" },
    { item_definition_indexes::WEAPON_M249, "m249" },
    { item_definition_indexes::WEAPON_M4A1_SILENCER, "m4a1-s" },
    { item_definition_indexes::WEAPON_M4A1, "m4a4" },
    { item_definition_indexes::WEAPON_MAC10, "mac-10" },
    { item_definition_indexes::WEAPON_MAG7, "mag-7" },
    { item_definition_indexes::WEAPON_MP7, "mp7" },
    { item_definition_indexes::WEAPON_MP5SD, "mp5" },
    { item_definition_indexes::WEAPON_MP9, "mp9" },
    { item_definition_indexes::WEAPON_NEGEV, "negev" },
    { item_definition_indexes::WEAPON_NOVA, "nova" },
    { item_definition_indexes::WEAPON_HKP2000, "p2000" },
    { item_definition_indexes::WEAPON_P250, "p250" },
    { item_definition_indexes::WEAPON_P90, "p90" },
    { item_definition_indexes::WEAPON_BIZON, "pp-bizon" },
    { item_definition_indexes::WEAPON_REVOLVER, "r8 revolver" },
    { item_definition_indexes::WEAPON_SAWEDOFF, "sawed-off" },
    { item_definition_indexes::WEAPON_SCAR20, "scar-20" },
    { item_definition_indexes::WEAPON_SSG08, "ssg 08" },
    { item_definition_indexes::WEAPON_SG556, "sg 553" },
    { item_definition_indexes::WEAPON_TEC9, "tec-9" },
    { item_definition_indexes::WEAPON_UMP45, "ump-45" },
    { item_definition_indexes::WEAPON_USP_SILENCER, "usp-s" },
    { item_definition_indexes::WEAPON_XM1014, "xm1014" },
};

void menu::initialize() {
    ImGui::CreateContext();
    ImGui_ImplWin32_Init(hooks::window);

    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();

    style.ScrollbarSize = 9.0f;
    style.WindowRounding = 0.f;
    style.Colors[ImGuiCol_WindowBg] = ImColor(20.f / 255.f, 20.f / 255.f, 20.f / 255.f, style.Alpha);
    style.Colors[ImGuiCol_Border] = ImColor(61.f / 255.f, 65.f / 255.f, 137.f / 255.f, style.Alpha);
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(52.f / 255.f, 50.f / 255.f, 59.f / 255.f, style.Alpha);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(62.f / 255.f, 66.f / 255.f, 141.f / 255, style.Alpha);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(82.f / 255.f, 86.f / 255.f, 161.f / 255, style.Alpha);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(62.f / 255.f, 66.f / 255.f, 141.f / 255, style.Alpha);
    
    style.Colors[ImGuiCol_ChildBg] = ImVec4(52.f / 255.f, 50.f / 255.f, 59.f / 255.f, style.Alpha);

    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(62.f / 255.f, 66.f / 255.f, 141.f / 255, style.Alpha);
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(62.f / 255.f, 66.f / 255.f, 141.f / 255, style.Alpha);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(62.f / 255.f, 66.f / 255.f, 141.f / 255, style.Alpha);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(65.f / 255.f, 64.f / 255.f, 74.f / 255, style.Alpha);

    style.Colors[ImGuiCol_Button] = ImVec4(62.f / 255.f, 66.f / 255.f, 141.f / 255, style.Alpha);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(82.f / 255.f, 86.f / 255.f, 161.f / 255, style.Alpha);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(62.f / 255.f, 66.f / 255.f, 141.f / 255, style.Alpha);

    style.Colors[ImGuiCol_Header] = ImVec4(62.f / 255.f, 66.f / 255.f, 141.f / 255, style.Alpha);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(82.f / 255.f, 86.f / 255.f, 161.f / 255, style.Alpha);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(62.f / 255.f, 66.f / 255.f, 141.f / 255, style.Alpha);

    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(85.f / 255.f, 84.f / 255.f, 94.f / 255, style.Alpha);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(45.f / 255.f, 44.f / 255.f, 54.f / 255, style.Alpha);
    style.Colors[ImGuiCol_PopupBg] = ImVec4(45.f / 255.f, 44.f / 255.f, 54.f / 255.f, style.Alpha);

    style.Colors[ImGuiCol_PlotHistogram] = ImVec4(62.f / 255.f, 66.f / 255.f, 141.f / 255, style.Alpha);

    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;
    io.LogFilename = nullptr;
    io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

    menu::settings::tahoma = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\bahnschrift.ttf", 14.0f, nullptr);
    menu::settings::tahoma2 = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\bahnschrift.ttf", 18.0f, nullptr);
    menu::settings::tahoma3 = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\bahnschrift.ttf", 12.0f, nullptr);

    ImGuiFreeType::BuildFontAtlas(io.Fonts, ImGuiFreeType::ForceAutoHint);
}

void menu::widgets::current_weapon_button(float width) {
    if (!interfaces::engine->is_connected() || !csgo::local_player || !csgo::local_player->is_alive()) {
        ImGui::Button(("current"), ImVec2(width, 0.0f));
        return;
    }

    auto weapon = csgo::local_player->active_weapon();
    if (!weapon || !weapon->is_weapon()) {
        ImGui::Button(("current"), ImVec2(width, 0.0f));
        return;
    }

    if (ImGui::Button(("current"), ImVec2(width, 0.0f))) {
        int wpn_idx = weapon->item_definition_index();
        auto wpn_it = std::find_if(weapon_names.begin(), weapon_names.end(), [wpn_idx](const menu::settings::weapon_name_t& a) {
            return a.definition_index == wpn_idx;
            });
        if (wpn_it != weapon_names.end()) {
            menu::settings::weapon_index = wpn_idx;
            menu::settings::weapon_vector_index = std::abs(std::distance(weapon_names.begin(), wpn_it));
        }
    }
}

void menu::widgets::visuals_preview() {
    std::vector<std::pair<std::string, ImColor>> flags_vector;
    ImVec2 p = ImGui::GetCursorScreenPos();
    ImColor c = ImColor(32, 114, 247);
    const auto cur_window = ImGui::GetCurrentWindow();
    const ImVec2 w_pos = cur_window->Pos;
    auto position = 46;

    int box_w = 210;
    int box_h = 320;

    //box
    if (configuration::settings::visuals_player_box)
        cur_window->DrawList->AddRect(ImVec2(w_pos.x + 72, w_pos.y + 50), ImVec2(w_pos.x + box_w, w_pos.y + box_h), ImGui::GetColorU32(ImGuiCol_Text));

    //hp
    if (configuration::settings::visuals_player_health)
        cur_window->DrawList->AddRectFilled(ImVec2(w_pos.x + 69, w_pos.y + 50), ImVec2(w_pos.x + 71, w_pos.y + box_h), ImGui::GetColorU32(ImVec4(83 / 255.f, 200 / 255.f, 84 / 255.f, 255 / 255.f)));

    //armor
    if (configuration::settings::visuals_player_armor_bar)
        cur_window->DrawList->AddRectFilled(ImVec2(w_pos.x + 72, w_pos.y + box_h + 3), ImVec2(w_pos.x + 210, w_pos.y + box_h + 1), ImGui::GetColorU32(ImVec4(0 / 255.f, 80 / 255.f, 255 / 255.f, 255 / 255.f)));

    //name
    if (configuration::settings::visuals_player_name)
        cur_window->DrawList->AddText(ImVec2(w_pos.x + 130, w_pos.y + 39), ImGui::GetColorU32(ImGuiCol_Text), ("name"));

    //weapon
    if (configuration::settings::visuals_player_weapon)
        cur_window->DrawList->AddText(ImVec2(w_pos.x + 125, w_pos.y + box_h + 1), ImGui::GetColorU32(ImGuiCol_Text), ("weapon"));

    if (configuration::settings::visuals_flags[0])
        flags_vector.push_back(std::pair<std::string, ImColor>(("bot"), ImColor(255, 0, 0)));

    if (configuration::settings::visuals_flags[1])
        flags_vector.push_back(std::pair<std::string, ImColor>(("hk"), ImColor(255, 255, 255)));

    if (configuration::settings::visuals_flags[2])
        flags_vector.push_back(std::pair<std::string, ImColor>(std::string(("$16000")), ImColor(120, 180, 10)));

    if (configuration::settings::visuals_flags[3])
        flags_vector.push_back(std::pair<std::string, ImColor>(std::string(("zoom")), ImColor(80, 160, 200, 255)));

    if (configuration::settings::visuals_flags[4])
        flags_vector.push_back(std::pair<std::string, ImColor>(std::string(("flashed")), ImColor(255, 255, 255)));

    if (configuration::settings::visuals_flags[5])
        flags_vector.push_back(std::pair<std::string, ImColor>(std::string(("desync")), ImColor(255, 0, 0)));

    if (configuration::settings::visuals_flags[6])
        flags_vector.push_back(std::pair<std::string, ImColor>(std::string(("defusing")), ImColor(247, 202, 24)));

    for (auto text : flags_vector) {
        cur_window->DrawList->AddText(ImVec2(w_pos.x + box_w + 2, w_pos.y + position), ImColor(text.second), text.first.c_str());
        position += 7;
    }
}

void menu::tabs::legit() {
    static const char* aimbot_type[] = { "on shot", "always" };
    static const char* hitboxes[4] = { "head", "chest", "hands", "legs" };
    static const char* trigger_type[] = { "on key", "always" };

    static int tab = 0;

    ImGui::SetCursorPos({ 0 ,50 });
    if (ImGui::SubTab("aimbot", "", { 330,37 }, tab == 0 ? false : true)) {
        tab = 0;
        update_animation_alpha();
    }

    ImGui::SetCursorPos({ 330,50 });
    if (ImGui::SubTab("triggerbot", "", { 330,37 }, tab == 1 ? false : true)) {
        tab = 1;
        update_animation_alpha();
    }


    auto settings = &configuration::settings::aimbot[menu::settings::weapon_index];

    if (tab == 0) {

        ImGui::SetCursorPos({ GROUPBOX_FIRST_POSITION });
        ImGui::BeginChild("aim assist", ImVec2(GROUPBOX_SIZE), false, NULL);

        ImGui::PushFont(menu::settings::tahoma3);

        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 7);
        ImGui::Checkbox(("backtrack"), &configuration::settings::aimbot_backtrack_enable); tool_tip("abuse lag compensation to move players back in time.");

        ImGui::Checkbox(("ping spike"), &configuration::settings::aimbot_backtrack_ping_spike); tool_tip("extend backtrack window.");

        ImGui::SliderFloat(("ping spike value"), &configuration::settings::aimbot_backtrack_ping_spike_value, 0.05f, 0.2f); tool_tip("extended backtracking value.");

        ImGui::Checkbox(("aim assist"), &settings->enabled); tool_tip("enable aiming assistance features.");

        ImGui::Combo(("assist type"), &configuration::settings::aimbot_type, aimbot_type, IM_ARRAYSIZE(aimbot_type)); tool_tip("type of aiming assistance.");

        ImGui::Checkbox(("auto wall"), &configuration::settings::aimbot_target_visible); tool_tip("target players behind penetrable walls.");

        ImGui::Checkbox(("auto pistol"), &configuration::settings::aimbot_auto_pistol); tool_tip("fire pistols like automatic rifles.");

        ImGui::SliderFloat(("min damage"), &configuration::settings::aimbot_auto_wall_mim_damage, 1.f, 100.f); tool_tip("minimum damage to target someone behind wall.");

        ImGui::SliderInt(("delay after kill"), &configuration::settings::aimbot_delay_after_kill, 0, 350); tool_tip("stop aimbot for x ms after killing someone.");

        ImGui::SliderFloat(("field of view"), &settings->fov, 1.f, 60.f); tool_tip("the maximum field of view distance at which aimbot engages.");

        ImGui::SliderFloat(("smoothing"), &settings->smooth, 1.f, 20.f); tool_tip("smooth aimbot speed in order to look more legit.");

        ImGui::SliderFloat(("compensate recoil"), &settings->rcs, 0.f, 1.f); tool_tip("compensate current weapon recoil.");

        ImGui::MultiCombo(("hitboxes"), hitboxes, settings->hitboxes, 4); tool_tip("body parts on which aimbot works.");


        ImGui::PopFont();

        ImGui::EndChild();

        ImGui::SetCursorPos({ GROUPBOX_SECOND_POSITION });
        ImGui::BeginChild("settings", ImVec2(GROUPBOX_SIZE), false, NULL);

        ImGui::PushFont(menu::settings::tahoma3);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 7);
        if (ImGui::Combo(("weapon selection"), &menu::settings::weapon_vector_index,
            [](void* data, int32_t idx, const char** out_text) {
                auto vec = reinterpret_cast<std::vector< menu::settings::weapon_name_t >*>(data);
                *out_text = vec->at(idx).name;
                return true;
            }, (void*)(&weapon_names), weapon_names.size())) {
            menu::settings::weapon_index = weapon_names[menu::settings::weapon_vector_index].definition_index;
        }

        menu::widgets::current_weapon_button(200.f);

        ImGui::Checkbox(("smoke check"), &configuration::settings::aimbot_target_smoke); tool_tip("aimbot target players behind smoke.");
        ImGui::Checkbox(("team check"), &configuration::settings::aimbot_target_team); tool_tip("aimbot target teammates.");
        ImGui::Checkbox(("jump check"), &configuration::settings::aimbot_target_in_air); tool_tip("aimbot target jumping players.");

        ImGui::PopFont();

        ImGui::EndChild();

    }
    else if (tab == 1) {


        ImGui::SetCursorPos({ GROUPBOX_FIRST_POSITION });
        ImGui::BeginChild("triggerbot", ImVec2(GROUPBOX_SIZE), false, NULL);

        ImGui::PushFont(menu::settings::tahoma3);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 7);
        ImGui::Checkbox(("trigger bot"), &configuration::settings::triggerbot_enable); tool_tip("automatically shot when crosshair is on enemy.");
        ImGui::Combo(("trigger type"), &configuration::settings::aimbot_trigger_type, trigger_type, IM_ARRAYSIZE(trigger_type)); tool_tip("types of triggerbot.");

        if (configuration::settings::aimbot_trigger_type == 0)
            ImGui::Hotkey("", &configuration::settings::aimbot_trigger_key, ImVec2(204, 20)); tool_tip("triggerbot works only when chosen key is being held.");

        auto settings = &configuration::settings::triggerbot[menu::settings::weapon_index];
        ImGui::SliderFloat(("hitchance"), &settings->hitchance, 1.f, 99.f, "%.f"); tool_tip("minimum accuracy needed for triggerbot to fire.");
        ImGui::SliderInt(("delay"), &settings->delay, 1, 30); tool_tip("time needed for triggerbot to fire (ms).");

        ImGui::PopFont();

        ImGui::EndChild();

        ImGui::SetCursorPos({ GROUPBOX_SECOND_POSITION });
        ImGui::BeginChild("settings", ImVec2(GROUPBOX_SIZE), false, NULL);

        ImGui::PushFont(menu::settings::tahoma3);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 7);
        if (ImGui::Combo(("weapon selection"), &menu::settings::weapon_vector_index, 
            [](void* data, int32_t idx, const char** out_text) {
                auto vec = reinterpret_cast<std::vector< menu::settings::weapon_name_t >*>(data);
                *out_text = vec->at(idx).name;
                return true;
            }, (void*)(&weapon_names), weapon_names.size())) {
            menu::settings::weapon_index = weapon_names[menu::settings::weapon_vector_index].definition_index;
        }

        menu::widgets::current_weapon_button(200.f);

        ImGui::PopFont();

        ImGui::EndChild();


    }
}

void menu::tabs::visuals() {
    static int visuals_tab = 0;
    static const char* flags[7] = { "bot", "armor", "money", "scoped", "flashed", "desync", "defusing" };
    static const char* visuals_toggle_type[] = { "always", "on key hold", "on key toggle" };
    static const char* c4_type[] = { "text", "box", "screen overlay" };
    static const char* danger_zone_items[] = { "no grass", "cash", "cases", "armor", "npc", "weapons", "health shots", "misc items" };
    static const char* projectiles_type[] = { "text", "box" };
    static const char* chams_types[] = { "textured", "flat" };

    ImGui::SetCursorPos({ 0,50 });
    if (ImGui::SubTab("players", "", { 220,37 }, visuals_tab == 0 ? false : true)) {
        visuals_tab = 0;
        update_animation_alpha();
    }

    ImGui::SetCursorPos({ 220,50 });
    if (ImGui::SubTab("chams", "", { 220,37 }, visuals_tab == 1 ? false : true)) {
        visuals_tab = 1;
        update_animation_alpha();
    }

    ImGui::SetCursorPos({ 220 * 2,50 });
    if (ImGui::SubTab("other", "", { 220,37 }, visuals_tab == 2 ? false : true)) {
        visuals_tab = 2;
        update_animation_alpha();
    }

    
    if (visuals_tab == 0) {
        ImGui::SetCursorPos({ GROUPBOX_FIRST_POSITION });
        ImGui::BeginChild("players", ImVec2(GROUPBOX_SIZE), false, NULL);

        ImGui::PushFont(menu::settings::tahoma3);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 7);
        ImGui::Checkbox(("enemies visuals"), &configuration::settings::visuals_enable); tool_tip("enable visual options for enemies.");
        ImGui::Checkbox(("teammates visuals"), &configuration::settings::visuals_enable_team); tool_tip("enable visual options for teammates.");

        ImGui::Combo(("activation type"), &configuration::settings::visuals_toggle_type, visuals_toggle_type, IM_ARRAYSIZE(visuals_toggle_type));

        if (configuration::settings::visuals_toggle_type > 0) {
            ImGui::PushID(("visuals_toggle_key")); tool_tip("visual options enabled only when chosen key is being held.");
            ImGui::Hotkey("", &configuration::settings::visuals_toggle_key, ImVec2(204, 20));
            ImGui::PopID();
        }

        ImGui::Checkbox(("fade animation"), &configuration::settings::visuals_fade); tool_tip("fade in/out dormant players smoothly.");
        ImGui::Checkbox(("glow"), &configuration::settings::visuals_glow_enable); tool_tip("display glow effect on entities.");

        ImGui::ColorEdit4(("##glow color picker"), configuration::settings::visuals_glow_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);

        ImGui::Checkbox(("name"), &configuration::settings::visuals_player_name); tool_tip("display player's name.");
        ImGui::ColorEdit4(("##name color picker"), configuration::settings::visuals_name_color, ImGuiColorEditFlags_NoInputs);

        ImGui::Checkbox(("box"), &configuration::settings::visuals_player_box); tool_tip("display player's box.");
        ImGui::ColorEdit4(("##box color picker"), configuration::settings::visuals_box_color, ImGuiColorEditFlags_NoInputs);

        ImGui::Checkbox(("skeleton"), &configuration::settings::visuals_player_skeleton); tool_tip("display player's backtracked skeleton.");
        ImGui::Checkbox(("health"), &configuration::settings::visuals_player_health); tool_tip("display player's current health.");
        ImGui::Checkbox(("armor"), &configuration::settings::visuals_player_armor_bar); tool_tip("display player's current armor.");
        ImGui::Checkbox(("weapon"), &configuration::settings::visuals_player_weapon); tool_tip("display player's current weapon.");
        ImGui::ColorEdit4(("##weapon color picker"), configuration::settings::visuals_weapon_color, ImGuiColorEditFlags_NoInputs);

        ImGui::MultiCombo(("flags"), flags, configuration::settings::visuals_flags, 7); tool_tip("additional information per player.");

        ImGui::PopFont();

        ImGui::EndChild();

        ImGui::SetCursorPos({ GROUPBOX_SECOND_POSITION });
        ImGui::BeginChild("preview", ImVec2(GROUPBOX_SIZE), false, NULL);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 7);
        ImGui::PushFont(menu::settings::tahoma3);

        menu::widgets::visuals_preview();

        ImGui::PopFont();

        ImGui::EndChild();

    }
    else if (visuals_tab == 1) {

        ImGui::SetCursorPos({ GROUPBOX_FIRST_POSITION });
        ImGui::BeginChild("players", ImVec2(GROUPBOX_SIZE), false, NULL);

        ImGui::PushFont(menu::settings::tahoma3);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 7);
        ImGui::Checkbox(("health based"), &configuration::settings::visuals_chams_health_based); tool_tip("enable material color based on player's hp.");
        ImGui::Checkbox(("visible first material"), &configuration::settings::visuals_chams_enable); tool_tip("enable first visible material.");
        ImGui::ColorEdit4(("##chams color picker"), configuration::settings::visuals_chams_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
        ImGui::PushID(30);

        ImGui::Combo(("type"), &configuration::settings::visuals_chams_type, chams_types, IM_ARRAYSIZE(chams_types)); tool_tip("select type of first visible material.");

        ImGui::PopID();


        ImGui::Checkbox(("hidden first material"), &configuration::settings::visuals_chams_hidden_enable); tool_tip("enable first invisible material.");
        ImGui::ColorEdit4(("##chams hidden color picker"), configuration::settings::visuals_chams_color_hidden, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
        ImGui::PushID(43);
        ImGui::Combo(("type"), &configuration::settings::visuals_chams_type_hidden, chams_types, IM_ARRAYSIZE(chams_types)); tool_tip("select type of first invisible material.");
        ImGui::PopID();

        ImGui::Checkbox(("backtrack"), &configuration::settings::visuals_backtrack_chams); tool_tip("enable visible/invisible lag compensation records material.");
        ImGui::ColorEdit4(("##backtrack chams color picker"), configuration::settings::visuals_backtrack_chams_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);

        ImGui::PopFont();

        ImGui::EndChild();

        ImGui::SetCursorPos({ GROUPBOX_SECOND_POSITION });
        ImGui::BeginChild("hands", ImVec2(GROUPBOX_SIZE), false, NULL);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 7);
        ImGui::PushFont(menu::settings::tahoma3);

        ImGui::Checkbox(("first material"), &configuration::settings::visuals_chams_hands_first_material); tool_tip("enable local player's hands first material.");
        ImGui::ColorEdit4(("##hands chams color picker"), configuration::settings::visuals_chams_hands_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
        ImGui::PushID(31);
        ImGui::Combo(("type"), &configuration::settings::visuals_chams_hands_first_material_type, chams_types, IM_ARRAYSIZE(chams_types)); tool_tip("select type of local player's first hands material.");
        ImGui::PopID();

        ImGui::PopFont();

        ImGui::EndChild();

    }
    else if (visuals_tab == 2) {

        ImGui::SetCursorPos({ GROUPBOX_FIRST_POSITION });
        ImGui::BeginChild("world", ImVec2(GROUPBOX_SIZE), false, NULL);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 7);
        ImGui::PushFont(menu::settings::tahoma3);


        ImGui::Checkbox(("night mode"), &configuration::settings::visuals_nightmode); tool_tip("modulate world color and skybox in order to look like night.");

        ImGui::Checkbox(("planted c4"), &configuration::settings::visuals_bomb_timer); tool_tip("display various info about planted bomb.");
        ImGui::ColorEdit4(("##timer color picker"), configuration::settings::visuals_bomb_timer_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
        ImGui::MultiCombo(("planted c4 type"), c4_type, configuration::settings::planted_c4_type, 3); tool_tip("select type of bomb visuals.");

        ImGui::Checkbox(("projectiles"), &configuration::settings::visuals_projectiles); tool_tip("display position of thrown nades.");
        ImGui::MultiCombo(("projectiles type"), projectiles_type, configuration::settings::visuals_projectiles_type, 2); tool_tip("select type of nades visuals.");

        ImGui::Checkbox(("dropped weapons"), &configuration::settings::visuals_dropped_weapons); tool_tip("display position of dropped weapons.");
        ImGui::ColorEdit4(("##dropped color picker"), configuration::settings::visuals_dropped_weapons_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
        ImGui::MultiCombo(("dropped weapons type"), c4_type, configuration::settings::visuals_dropped_weapons_type, 2); tool_tip("select type of dropped weapons visuals.");

        ImGui::Checkbox(("danger zone"), &configuration::settings::visuals_danger_zone_enable); tool_tip("display visual options specified for danger zone game mode.");
        ImGui::MultiCombo(("danger zone items"), danger_zone_items, configuration::settings::visuals_danger_zone_items, 8); tool_tip("select items to display in danger zone mode.");
        ImGui::SliderInt(("danger zone render distance"), &configuration::settings::visuals_danger_zone_render_distance, 500, 5000); tool_tip("the maximum distance at which danger zone items render.");

        ImGui::PopFont();

        ImGui::EndChild();

        ImGui::SetCursorPos({ GROUPBOX_SECOND_POSITION });
        ImGui::BeginChild("misc", ImVec2(GROUPBOX_SIZE), false, NULL);

        ImGui::PushFont(menu::settings::tahoma3);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 7);
        ImGui::Checkbox(("anti hit indicator"), &configuration::settings::visuals_draw_anti_hit_lines); tool_tip("display legit anti aim world/screen position of fake and real angle.");
        ImGui::Checkbox(("spectators"), &configuration::settings::misc_spectators); tool_tip("display spectators window.");
        ImGui::SliderFloat(("thirdperson distance"), &configuration::settings::visuals_thirdperson_distance, 50.f, 150.f); tool_tip("thirdperson camera view distance.");
        ImGui::SliderFloat(("render fov"), &configuration::settings::visuals_fov, 0.f, 60.f); tool_tip("first person field of view distance.");
        ImGui::SliderFloat(("view model fov"), &configuration::settings::visuals_view_model_fov, 0.f, 60.f); tool_tip("local player's first person view model position distance.");
        ImGui::Checkbox(("view model offsets"), &configuration::settings::visuals_view_model_changer_enable); tool_tip("enable local player's view model position modulation.");
        ImGui::SliderFloat(("view model x"), &configuration::settings::visuals_viewmodel_x, -15.0f, 15.0f); tool_tip("local player's view model x axis position.");
        ImGui::SliderFloat(("view model y"), &configuration::settings::visuals_viewmodel_y, -15.0f, 15.0f); tool_tip("local player's view model y axis position.");
        ImGui::SliderFloat(("view model z"), &configuration::settings::visuals_viewmodel_z, -15.0f, 15.0f); tool_tip("local player's view model z axis position.");

        ImGui::PopFont();

        ImGui::EndChild();

    }
}

void menu::tabs::misc() {
    static const char* event_logger[] = { "damage dealt", "purchases" };
    static const char* agent_ct[] = { "Lt. Commander Ricksaw | NSWC SEAL", "'Two Times' McCoy | USAF TACP",
"Buckshot | NSWC SEAL", "Seal Team 6 Soldier | NSWC SEAL", "3rd Commando Company | KSK", "Michael Syfers  | FBI Sniper",
"Markus Delrow | FBI HRT", "Operator | FBI SWAT", "Special Agent Ava | FBI", "B Squadron Officer | SAS" };
    static const char* agent_t[] = { "Blackwolf | Sabre", "Rezan The Ready | Sabre", "Maximus | Sabre", "Dragomir | Sabre",
        "'The Doctor' Romanov | Sabre", "Slingshot | Phoenix", "Enforcer | Phoenix", "Soldier | Phoenix", "The Elite Mr. Muhlik | Elite Crew",
        "Prof.Shahmat | Elite Crew", "Osiris | Elite Crew", "Ground Rebel  | Elite Crew" };

    std::string data_center_list[] = { "", "syd", "vie", "gru", "scl", "dxb", "par", "fra", "hkg",
"maa", "bom", "tyo", "lux", "ams", "limc", "man", "waw", "sgp", "jnb",
"mad", "sto", "lhr", "atl", "eat", "ord", "lax", "mwh", "okc", "sea", "iad" };

    const char* data_center_list_names[] = { "don't force", "Australia", "Austria", "Brazil", "Chile", "Dubai", "France", "Germany", "Hong Kong", "India (Chennai)", "India (Mumbai)", "Japan", "Luxembourg",
    "Netherlands", "Peru", "Philipines", "Poland", "Singapore", "South Africa", "Spain", "Sweden", "UK", "USA (Atlanta)", "USA (Seattle)", "USA (Chicago)", "USA (Los Angeles)", "USA (Moses Lake)",
    "USA (Oklahoma)", "USA (Seattle)", "USA (Washington DC)" };

    static int misc_tab = 0;

    ImGui::SetCursorPos({ 0 ,50 });
    if (ImGui::SubTab("misc", "", { 660,37 }, misc_tab == 0 ? false : true))
        misc_tab = 0;

    if (misc_tab == 0) {


        ImGui::SetCursorPos({ GROUPBOX_FIRST_POSITION });
        ImGui::BeginChild("other", ImVec2(GROUPBOX_SIZE), false, ImGuiWindowFlags_AlwaysVerticalScrollbar);

        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 7);
        ImGui::PushFont(menu::settings::tahoma3);
        ImGui::Checkbox(("bunny hop"), &configuration::settings::movement_bunnyhop); tool_tip("automatically jumps for you, increases movement speed.");
        ImGui::SliderInt(("bunny hop hitchance"), &configuration::settings::movement_bunnyhop_hitchance, 0, 100); tool_tip("minimum chance needed to hit next hop.");
        ImGui::Checkbox(("slide walk"), &configuration::settings::misc_slide_walk); tool_tip("remove moving animation, makes local player sliding, visible for other players.");
        ImGui::Checkbox(("clan tag spammer"), &configuration::settings::misc_clantag_spammer); tool_tip("set custom clantag.");
        ImGui::Checkbox(("synced clan tag"), &configuration::settings::misc_synced_clantag); tool_tip("synchronise clantag with other aristois users.");
        ImGui::PushItemWidth(200.f);
        ImGui::InputText("##clantagtext", configuration::settings::misc_clantag_text, IM_ARRAYSIZE(configuration::settings::misc_clantag_text));
        ImGui::Checkbox(("name spammer"), &configuration::settings::misc_name_spammer); tool_tip("change local player's name to advertise cheat and then goes back to the original.");
        ImGui::Checkbox(("reveal server ranks"), &configuration::settings::misc_reveal_server_ranks); tool_tip("show player ranks in scoreboard in competitive mode.");
        ImGui::Checkbox(("force radar"), &configuration::settings::visuals_force_radar); tool_tip("show enemies positions on in game radar.");
        ImGui::Checkbox(("force crosshair"), &configuration::settings::misc_force_crosshair); tool_tip("display crosshair while holding sniper rifle.");
        ImGui::Checkbox(("reduce flashbang effect"), &configuration::settings::misc_remove_flashbang); tool_tip("enable flashbang grenade effect modulation.");
        ImGui::SliderFloat(("effect removal strength"), &configuration::settings::misc_remove_flashbang_alpha, 0.0f, 255.0f); tool_tip("modulate flashbang grenade effect.");

        ImGui::Checkbox(("hit marker"), &configuration::settings::misc_hitmarker); tool_tip("display cod style hitmarker when you hit an enemy.");
        if (configuration::settings::misc_hitmarker) {
            ImGui::Checkbox(("hit sound"), &configuration::settings::misc_hitmarker_sound); tool_tip("emit metalic sound when you hit an enemy.");
            ImGui::Checkbox(("kill effect"), &configuration::settings::misc_hitmarker_screen_effect); tool_tip("display healthshot effect when you hit an enemy.");
        }

        ImGui::Checkbox(("kill message"), &configuration::settings::misc_killspam); tool_tip("send chat message after killing an enemy.");
        ImGui::PushItemWidth(200.f);
        ImGui::InputText("##killspamtext", configuration::settings::misc_kill_message, IM_ARRAYSIZE(configuration::settings::misc_kill_message));
        ImGui::PopItemWidth();

        ImGui::MultiCombo(("event logger"), event_logger, configuration::settings::misc_event_logger, 2); tool_tip("log various events at the top left cornder of your screen.");
        ImGui::ColorEdit4(("##test color picker"), configuration::settings::gui_menu_accent, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);

        ImGui::Checkbox(("anti hit"), &configuration::settings::aimbot_anti_hit_enable); tool_tip("enable legit anti aim.");
        ImGui::SliderInt(("choked packets"), &configuration::settings::aimbot_anti_hit_choked_packets, 1, 5); tool_tip("fake lag, amount of choked user commands");


        ImGui::Checkbox(("matchmaking region"), &configuration::settings::misc_matchmaking_region_enable); tool_tip("enable matchmaking region selector.");
        ImGui::Combo(("select matchmaking region"), &configuration::settings::misc_matchmaking_region, data_center_list_names, IM_ARRAYSIZE(data_center_list_names)); tool_tip("select matchmaking region u want to play on.");

     
        static std::string* force_relay_cluster_value = *(std::string**)(utilities::pattern_scan("steamnetworkingsockets.dll", "B8 ? ? ? ? B9 ? ? ? ? 0F 43") + 1);
        if (configuration::settings::misc_matchmaking_region_enable && !interfaces::engine->is_connected() && !interfaces::engine->is_in_game())
            *force_relay_cluster_value = data_center_list[configuration::settings::misc_matchmaking_region];


        //ImGui::ColorEdit4(("##second chams color picker"), configuration::settings::visuals_chams_hands_second_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoTooltip);
        
        ImGui::PopFont();

        ImGui::EndChild();

        ImGui::SetCursorPos({ GROUPBOX_SECOND_POSITION });
        ImGui::BeginChild("changers", ImVec2(GROUPBOX_SIZE), false, NULL);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 7);
        ImGui::PushFont(menu::settings::tahoma3);
        
        ImGui::Checkbox(("agent changer"), &configuration::settings::misc_agent_changer_enable); tool_tip("enable local player's agnet model changer");
        ImGui::Combo(("counter-terrorist agent"), &configuration::settings::misc_agent_ct, agent_ct, IM_ARRAYSIZE(agent_ct)); tool_tip("select local player's counter-terrorist agent");
        ImGui::Combo(("terrorist agent"), &configuration::settings::misc_agent_t, agent_t, IM_ARRAYSIZE(agent_t)); tool_tip("select local player's terrorist agent");

        ImGui::PopFont();

        
        ImGui::EndChild();


    }

}


void menu::tabs::config() {
    static int config_tab = 0;
    static const char* configs[] = { "1", "2", "3", "4", "5" };
    static const char* choices[]{ "  yes", "  no" };
    static int current_config = 0;

    ImGui::SetCursorPos({ 0 ,50 });
    if (ImGui::SubTab("settings", "", { 660,37 }, config_tab == 0 ? false : true))
        config_tab = 0;

    if (config_tab == 0) {


        ImGui::SetCursorPos({ GROUPBOX_FIRST_POSITION });
        ImGui::BeginChild("config", ImVec2(GROUPBOX_SIZE), false, NULL);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 7);
        ImGui::PushFont(menu::settings::tahoma3);

        ImGui::Combo(("configuration"), &current_config, configs, IM_ARRAYSIZE(configs));
        if (ImGui::Button(("save"), ImVec2(200.f, 20))) {
            ImGui::OpenPopup(("config save popup"));
        }

        if (ImGui::BeginPopup(("config save popup"))) {
            ImGui::Text(("are you sure you want to save selected config? "));

            for (auto i = 0; i < IM_ARRAYSIZE(choices); i++)
                if (ImGui::Selectable(choices[i]))
                    if (i == 0) {
                        configuration::save(configs[current_config]);
                        misc::notification_system::notify(std::string("successfully saved config at slot: ").append(std::to_string(current_config + 1)), color::white(configuration::settings::gui_menu_accent[3] * 255.f));
                    }

            ImGui::Spacing();

            ImGui::EndPopup();
        }

        if (ImGui::Button(("load"), ImVec2(200.f, 20))) {
            ImGui::OpenPopup(("config load popup"));
        }

        if (ImGui::BeginPopup(("config load popup"))) {
            ImGui::Text(("are you sure you want to load selected config? "));

            for (auto i = 0; i < IM_ARRAYSIZE(choices); i++)
                if (ImGui::Selectable(choices[i]))
                    if (i == 0) {
                        configuration::load(configs[current_config]);
                        misc::notification_system::notify(std::string("successfully loaded config at slot: ").append(std::to_string(current_config + 1)), color::white(configuration::settings::gui_menu_accent[3] * 255.f));
                    }

            ImGui::Spacing();

            ImGui::EndPopup();
        }

        if (ImGui::Button(("open configuration folder"), ImVec2(200.f, 20)))
            ShellExecuteW(0, L"open", L"C:/csgo_base/configs", NULL, NULL, SW_NORMAL);

        static int current_lua_item = -1;

        ImGui::Spacing();

        ImGui::Combo("lua scripts", &current_lua_item,
            [](void* vec, int idx, const char** out_text) {
                std::vector<std::string>* vector = reinterpret_cast<std::vector<std::string>*>(vec);
                if (idx < 0 || idx >= vector->size())return false;
                *out_text = vector->at(idx).c_str();
                return true;
            }, reinterpret_cast<void*>(&lua::scripts), lua::scripts.size());

        if (ImGui::Button("refresh", { 200.0f, 20.0f })) {
            lua::unload_all_scripts();
            lua::refresh_scripts();
        }

        if (ImGui::Button("reload all", { 200.0f, 20.0f }))
            lua::reload_all_scripts();

        if (ImGui::Button("unload All", { 200.0f, 20.0f }))
            lua::unload_all_scripts();

        if (current_lua_item != -1) {
            if (lua::loaded[current_lua_item]) {
                if (ImGui::Button("unload selected", { 200.0f, 20.0f }))
                    lua::unload_script(current_lua_item);
            }
            else {
                if (ImGui::Button("load selected", { 200.0f, 20.0f }))
                    lua::load_script(current_lua_item);
            }

        }


        ImGui::PopFont();

        ImGui::EndChild();

        ImGui::SetCursorPos({ GROUPBOX_SECOND_POSITION });
        ImGui::BeginChild("info", ImVec2(GROUPBOX_SIZE), false, NULL);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 7);
        ImGui::PushFont(menu::settings::tahoma3);

        ImGui::Text(("compilation date: " __DATE__ " - " __TIME__));
        ImGui::Text(std::strstr(GetCommandLineA(), "-insecure") ? ("insecure mode!") : ("insecure parameter not found."));

        for (auto hk : lua::hook_manager::get_hooks("on_menu")) {
            auto result = hk.func();
            if (!result.valid()) {
                sol::error err = result;
                interfaces::console->console_color_printf({ 255, 0, 0, 255 }, ("[lua] "));
                interfaces::console->console_printf(std::string(err.what()).append(" \n").c_str());
                utilities::console::log(err.what());
            }

        }

        ImGui::PopFont();

        ImGui::EndChild();


    }

}

void menu::render() {
    if (!menu::settings::open)
        return;

    auto& style = ImGui::GetStyle();
    auto flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0));

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0,0 });
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);

    ImGui::SetNextWindowSize(ImVec2(660, 530), ImGuiCond_Always);
    if (ImGui::Begin("aristois v2", &menu::settings::open, flags)) {
        menu::settings::alpha = std::clamp(menu::settings::alpha + menu::settings::animation_frequency * ImGui::GetIO().DeltaTime, 0.f, 1.f);
        style.Alpha = menu::settings::alpha;

        auto window_position = ImGui::GetWindowPos();
        auto draw_list = ImGui::GetWindowDrawList();

        draw_list->AddRectFilled({ window_position.x,window_position.y }, { window_position.x + 660,window_position.y + 530 }, ImColor(45, 44, 54));
        draw_list->AddRectFilled({ window_position.x,window_position.y + 502 }, { window_position.x + 660,window_position.y + 530 }, ImColor(61, 65, 137));

        draw_list->AddRectFilledMultiColor({ window_position.x,window_position.y }, { window_position.x + 660,window_position.y + 50 }, ImColor(33, 33, 45), ImColor(62, 66, 141), ImColor(62, 66, 141), ImColor(33, 33, 45));
        draw_list->AddRectFilledMultiColor({ window_position.x - 20,window_position.y + 50 }, { window_position.x + 660,window_position.y + 87 }, ImColor(62, 66, 141), ImColor(33, 33, 45), ImColor(33, 33, 45), ImColor(62, 66, 141));

        draw_list->AddRectFilledMultiColor({ window_position.x ,window_position.y + 85 }, { window_position.x + 660,window_position.y + 90 }, ImColor(62, 66, 141), ImColor(33, 33, 45), ImColor(33, 33, 45), ImColor(62, 66, 141));

        ImGui::PushFont(menu::settings::tahoma2);
        draw_list->AddText({ window_position.x + 15,window_position.y + 18 }, ImColor(255, 255, 255, 255), "aristois.me");
        ImGui::PopFont();

        ImGui::PushFont(menu::settings::tahoma2);
        ImGui::SetCursorPos({ 200,0 });
        if (ImGui::Tab("legit", "", { 110,50 }, current_tab == 0 ? true : false)) {
            current_tab = 0;
            update_animation_alpha();
        }

        ImGui::SetCursorPos({ 200 + 110,0 });
        if (ImGui::Tab("visuals", "", { 110,50 }, current_tab == 1 ? true : false)) {
            current_tab = 1;
            update_animation_alpha();
        }

        ImGui::SetCursorPos({ 200 + 110 * 2,0 });
        if (ImGui::Tab("misc", "", { 110,50 }, current_tab == 2 ? true : false)) {
            current_tab = 2;
            update_animation_alpha();

        }

        ImGui::SetCursorPos({ 200 + 110 * 3,0 });
        if (ImGui::Tab("settings", "", { 110,50 }, current_tab == 3 ? true : false)) {
            current_tab = 3;
            update_animation_alpha();
        }

        ImGui::PopFont();

        if (current_tab == 0)
            menu::tabs::legit();

        else if (current_tab == 1)
            menu::tabs::visuals();

        else if (current_tab == 2)
            menu::tabs::misc();

        else if (current_tab == 3)
            menu::tabs::config();

        if (!ImGui::IsAnyItemHovered() && !menu::widgets::tool_tips.empty())
            menu::widgets::tool_tips.clear();

        draw_list->AddText({ window_position.x + 10,window_position.y + 508 }, ImColor(255, 255, 255, 255), !menu::widgets::tool_tips.empty() ? menu::widgets::tool_tips.back().c_str() : "v2 for Counter-Strike: Global Offensive [developer]");
        ImGui::End();
    }
    ImGui::PopStyleColor();
    ImGui::PopStyleVar(3);
}
