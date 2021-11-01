#pragma once
#include "../../dependencies/utilities/csgo.hpp"
#include "../../dependencies/utilities/imgui/imgui_internal.h"
#include "../menu/config/config.hpp"

namespace menu {
    void initialize();
    void render();

    inline static int current_tab = 0;

    namespace settings {
        inline bool open = false;
        inline ImFont* tahoma;
        inline ImFont* tahoma2;
        inline ImFont* tahoma3;
        inline float alpha = 0.f;
        inline static float animation_frequency = 1.f / 0.25f;

        static int weapon_index = 7;
        static int weapon_vector_index = 0;
        struct weapon_name_t {
            constexpr weapon_name_t(int32_t definition_index, const char* name) : definition_index(definition_index), name(name) { }

            int32_t definition_index = 0;
            const char* name = nullptr;
        };
    }

    namespace widgets {
        void current_weapon_button(float width);
        void visuals_preview();
        inline std::vector<std::string> tool_tips;
    }

    namespace tabs {
        void legit();
        void visuals();
        void misc();
        void config();
    }
}