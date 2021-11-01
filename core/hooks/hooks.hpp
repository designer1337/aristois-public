#pragma once

class detour {
public:
	detour() = default;

	explicit detour(void* pFunction, void* pDetour)
		: _base(pFunction), _replace(pDetour) { }

	bool create(void* pFunction, void* pDetour) {
		_base = pFunction;

		if (_base == nullptr)
			return false;

		_replace = pDetour;

		if (_replace == nullptr)
			return false;

		auto status = MH_CreateHook(_base, _replace, &_original);

		if (status == MH_OK) {
			if (!this->replace())
				return false;
		}
		else {
			throw std::runtime_error("failed to create hook");
			return false;
		}

		return true;
	}

	bool replace() {
		if (_base == nullptr)
			return false;

		if (_is_hooked)
			return false;

		auto status = MH_EnableHook(_base);

		if (status == MH_OK)
			_is_hooked = true;
		else {
			throw std::runtime_error("failed to enable hook");
			return false;
		}

		return true;
	}

	template <typename arguments> arguments original() { return (arguments)_original; }
	inline bool is_hooked() { return _is_hooked; }

private:
	bool _is_hooked = false;
	void* _base = nullptr;
	void* _replace = nullptr;
	void* _original = nullptr;
};



namespace hooks {
	extern WNDPROC wndproc_original;
	extern HWND window;

	bool initialize();
	void release();

	LRESULT __stdcall wndproc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

	inline unsigned int get_virtual(void* _class, unsigned int index) { return static_cast<unsigned int>((*reinterpret_cast<int**>(_class))[index]); }

	namespace create_move {
		static constexpr auto index = 24;
		using fn = bool(__stdcall*)(float, c_usercmd*);
		bool __fastcall hook(void* ecx, void* edx, int input_sample_frametime, c_usercmd* cmd);
	};

	namespace paint_traverse {
		static constexpr auto index = 41;
		using fn = void(__thiscall*)(i_panel*, unsigned int, bool, bool);
		static void __stdcall hook(unsigned int panel, bool force_repaint, bool allow_force);
	}

	namespace view_model {
		static constexpr auto index = 35;
		using fn = float(__stdcall*)();
		static float __fastcall hook();
	}

	namespace draw_model_execute {
		static constexpr auto index = 21;
		using fn = void(__thiscall*)(iv_model_render*, i_material_render_context*, const draw_model_state_t&, const model_render_info_t&, matrix_t*);
		static void __stdcall hook(i_material_render_context* ctx, const draw_model_state_t& state, const model_render_info_t& info, matrix_t* bone_to_world);
		extern fn original;
	}

	namespace frame_stage_notify {
		static constexpr auto index = 37;
		using fn = void(__thiscall*)(i_base_client_dll*, int);
		static void __stdcall hook(int frame_stage);
	}

	namespace override_view {
		static constexpr auto index = 18;
		using fn = void* (__fastcall*)(i_client_mode*, void* _this, view_setup_t * setup);
		static void __fastcall hook(void* _this, void* _edx, view_setup_t* setup);
	}

	namespace do_post_screen_effects {
		static constexpr auto index = 44;
		using fn = int(__thiscall*)(void*, int);
		static int __stdcall hook(int value);
	}

	namespace sv_cheats_get_bool {
		static constexpr auto index = 13;
		using fn = bool(__thiscall*)(void*);
		static bool __fastcall hook(PVOID convar, int edx);
	}

	namespace list_leaves_in_box {
		static constexpr auto index = 6;
		using fn = int(__thiscall*)(void*, const vec3_t&, const vec3_t&, unsigned short*, int);
		static int __fastcall hook(void* bsp, void* edx, const vec3_t& mins, const vec3_t& maxs, unsigned short* list, int list_max);
	}

	namespace get_color_modulation {
		using fn = void(__thiscall*)(void*, float*, float*, float*);
		static void __fastcall hook(void* ecx, void* edx, float* r, float* g, float* b);
	}

	namespace is_using_static_prop_debug_modes {
		using fn = bool(__stdcall*)();
		static bool __stdcall hook();
	}

	namespace send_datagram {
		inline detour send_datagram_detour;
		int __fastcall hook(i_net_channel* net_channel, int edx, bf_write* datagram);
	}

	namespace present {
		static constexpr auto index = 17;
		long __stdcall hook(IDirect3DDevice9* device, RECT* source_rect, RECT* dest_rect, HWND dest_window_override, RGNDATA* dirty_region);
		using fn = long(__stdcall*)(IDirect3DDevice9*, RECT*, RECT*, HWND, RGNDATA*);
	}

	namespace reset {
		static constexpr auto index = 16;
		using fn = long(__stdcall*)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
		long __stdcall hook(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* present_parameters);
	}
}