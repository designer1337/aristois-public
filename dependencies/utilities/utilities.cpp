#include "utilities.hpp"
#include "csgo.hpp"
#include <psapi.h>

bool utilities::console::initialize(const char* window_name) {
	if (!AllocConsole())
		return false;

	const auto in = freopen_s(reinterpret_cast<FILE**>(stdin), "CONIN$", "r", stdin);

	if (in != 0)
		return false;

	const auto out = freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);

	if (out != 0)
		return false;

	if (!SetConsoleTitleA(window_name))
		return false;

	return true;
}

bool utilities::console::release(void) {
	const auto in = fclose(reinterpret_cast<FILE*>(stdin));

	if (in != 0)
		return false;

	const auto out = fclose(reinterpret_cast<FILE*>(stdout));

	if (out != 0)
		return false;

	if (!FreeConsole())
		return false;

	return true;
}

std::uint8_t* utilities::pattern_scan(const char* module_name, const char* signature) {
    const auto module_handle = GetModuleHandleA(module_name);

    if (!module_handle)
        return nullptr;

    static auto pattern_to_byte = [](const char* pattern) {
        auto bytes = std::vector<int>{};
        auto start = const_cast<char*>(pattern);
        auto end = const_cast<char*>(pattern) + std::strlen(pattern);

        for (auto current = start; current < end; ++current) {
            if (*current == '?') {
                ++current;

                if (*current == '?')
                    ++current;

                bytes.push_back(-1);
            }
            else {
                bytes.push_back(std::strtoul(current, &current, 16));
            }
        }
        return bytes;
    };

    auto dos_header = reinterpret_cast<PIMAGE_DOS_HEADER>(module_handle);
    auto nt_headers =
        reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<std::uint8_t*>(module_handle) + dos_header->e_lfanew);

    auto size_of_image = nt_headers->OptionalHeader.SizeOfImage;
    auto pattern_bytes = pattern_to_byte(signature);
    auto scan_bytes = reinterpret_cast<std::uint8_t*>(module_handle);

    auto s = pattern_bytes.size();
    auto d = pattern_bytes.data();

    for (auto i = 0ul; i < size_of_image - s; ++i) {
        bool found = true;

        for (auto j = 0ul; j < s; ++j) {
            if (scan_bytes[i + j] != d[j] && d[j] != -1) {
                found = false;
                break;
            }
        }
        if (found)
            return &scan_bytes[i];
    }

    return nullptr;
}

void utilities::apply_name(const char* name_to_change) {
	auto name = interfaces::console->get_convar("name");
	name->callbacks.size = 0;

	name->set_value(name_to_change);
}

void utilities::apply_clan_tag(const char* name) {
	using Fn = int(__fastcall*)(const char*, const char*);
	static auto apply_clan_tag_fn = reinterpret_cast<Fn>(utilities::pattern_scan("engine.dll", sig_apply_clan_tag));
	apply_clan_tag_fn(name, name);
};

bool utilities::is_behind_smoke(vec3_t start_pos, vec3_t end_pos) {
	typedef bool(__cdecl * line_goes_through_smoke) (vec3_t, vec3_t);
	static line_goes_through_smoke line_goes_through_smoke_fn = 0;

	if (!line_goes_through_smoke_fn)
		line_goes_through_smoke_fn = reinterpret_cast<line_goes_through_smoke>(utilities::pattern_scan("client.dll", sig_line_goes_thru_smoke));

	if (line_goes_through_smoke_fn)
		return line_goes_through_smoke_fn(start_pos, end_pos);
	return false;
}