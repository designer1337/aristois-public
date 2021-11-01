#pragma once

#include <map>
#include <filesystem>
#include "sol.hpp"

namespace lua {
	struct c_lua_hook {
		int scriptId;
		sol::protected_function func;
	};

	namespace hook_manager {
		void register_hook(std::string event_name, int script_id, sol::protected_function func);
		void un_register_hook(int script_id);
		extern std::vector<c_lua_hook> get_hooks(std::string event_name);
		inline std::map<std::string, std::vector<c_lua_hook>> hooks;
	};

	void init_state();
	void init_command();
	void unload();

	void load_script(int id);
	void unload_script(int id);
	void reload_all_scripts();
	void unload_all_scripts();
	void refresh_scripts();
	int get_script_id(std::string name);
	int get_script_id_by_path(std::string path);
	std::string get_script_path(std::string name);
	std::string get_script_path(int id);

	extern std::vector<std::filesystem::path> pathes;
	extern std::vector<bool> loaded;
	extern std::vector<std::string> scripts;
	extern lua_State* g_lua_state;
	extern bool g_unload_flag;
}
