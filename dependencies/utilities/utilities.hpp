#pragma once
#include "../../source-sdk/math/vectors.hpp"
#include <cstdarg>
#include <tuple>
#include <cmath>

namespace utilities {
	template<typename T, typename ...Args>
	constexpr auto call_virtual_method(void* class_base, int index, Args... args) {
		return ((*reinterpret_cast<T(__thiscall * **)(void*, Args...)>(class_base))[index])(class_base, args...);
	}

	std::uint8_t* pattern_scan(const char* module_name, const char* signature);
	void apply_name(const char* name_to_change);
	void apply_clan_tag(const char* name);
	bool is_behind_smoke(vec3_t start_pos, vec3_t end_pos);

	namespace console {
		bool initialize(const char* window_name);
		bool release(void);

		//wrapping this just to disable logging in release build - desinger
		template <typename ... Args>
		void log(char const* const format, Args const& ... args) {
#ifdef debug_build
			printf(format, args ...);
#endif
		}
	}

	namespace fnv_1a {
		template< typename S >
		struct fnv_internal;
		template< typename S >
		struct fnv1a;

		template< >
		struct fnv_internal< uint32_t > {
			constexpr static uint32_t default_offset_basis = 0x811C9DC5;
			constexpr static uint32_t prime = 0x01000193;
		};

		template< >
		struct fnv1a< uint32_t > : public fnv_internal< uint32_t > {
			constexpr static uint32_t hash(char const* string, const uint32_t val = default_offset_basis) {
				return (string[0] == '\0') ? val : hash(&string[1], (val ^ uint32_t(string[0])) * prime);
			}

			constexpr static uint32_t hash(wchar_t const* string, const uint32_t val = default_offset_basis) {
				return (string[0] == L'\0') ? val : hash(&string[1], (val ^ uint32_t(string[0])) * prime);
			}
		};
	}

	using fnv = fnv_1a::fnv1a< uint32_t >;

	template<typename T>
	class singleton {
	protected:
		singleton() { }
		~singleton() { }

		singleton(const singleton&) = delete;
		singleton& operator=(const singleton&) = delete;

		singleton(singleton&&) = delete;
		singleton& operator=(singleton&&) = delete;
	public:
		static T& get() {
			static T inst{};
			return inst;
		}
	};
}