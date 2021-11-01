#include "../dependencies/utilities/csgo.hpp"
#include "menu/config/config.hpp"
#include "features/features.hpp"
#include "hooks/event_listener.hpp"
#include "scripts/scripts.hpp"

unsigned long WINAPI initialize(void* instance) {
	while (!GetModuleHandleA("serverbrowser.dll"))
		Sleep(200);

	try {
		#ifdef debug_build
			utilities::console::initialize(("csgo_base debug console"));
		#endif

		lua::init_state();
		interfaces::initialize();
		hooks::initialize();
		event_listener.initialize();
		render::initialize();
		configuration::initialize();
		chams::initialize();
		lua::init_command();

		misc::notification_system::notify(("successfully injected."), color::white(configuration::settings::gui_menu_accent[3] * 255));
	}

	catch (const std::runtime_error & error) {
		MessageBoxA(NULL, error.what(), ("csgo_base error!"), MB_OK | MB_ICONERROR);
		FreeLibraryAndExitThread(static_cast<HMODULE>(instance), 0);
	}

	for (auto hk : lua::hook_manager::get_hooks("on_load")) {
		auto result = hk.func();
		if (!result.valid()) {
			sol::error err = result;
			utilities::console::log(err.what());
		}
	}

	while (!GetAsyncKeyState(VK_END))
		std::this_thread::sleep_for(std::chrono::milliseconds(50));

	FreeLibraryAndExitThread(static_cast<HMODULE>(instance), 0);
}

unsigned long WINAPI release() {
	for (auto hk : lua::hook_manager::get_hooks("on_unload")) {
		auto result = hk.func();
		if (!result.valid()) {
			sol::error err = result;
			utilities::console::log(err.what());
		}

	}

	hooks::release();
	event_listener.release();
	lua::unload();

#ifdef debug_build
	utilities::console::release();
#endif

	return TRUE;
}

std::int32_t WINAPI DllMain(void* instance, uintptr_t reason, void* reserved) {
	DisableThreadLibraryCalls(static_cast<HMODULE>(instance));

	switch (reason) {
	case DLL_PROCESS_ATTACH:
		if (auto handle = CreateThread(NULL, NULL, initialize, instance, NULL, NULL))
			CloseHandle(handle);
		break;

	case DLL_PROCESS_DETACH:
		release();
		break;
	}

	return true;
}