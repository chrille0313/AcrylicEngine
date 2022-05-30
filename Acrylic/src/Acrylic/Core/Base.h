#pragma once

#include <memory>

// Platform detection using predefined macros
#ifdef _WIN32
	/* Windows x64/x86 */
#ifdef _WIN64
	/* Windows x64  */
#define AC_PLATFORM_WINDOWS
#else
	/* Windows x86 */
#error "x86 Builds are not supported!"
#endif
#elif defined(__APPLE__) || defined(__MACH__)
#include <TargetConditionals.h>
/* TARGET_OS_MAC exists on all the platforms
 * so we must check all of them (in this order)
 * to ensure that we're running on MAC
 * and not some other Apple platform */
#if TARGET_IPHONE_SIMULATOR == 1
#error "IOS simulator is not supported!"
#elif TARGET_OS_IPHONE == 1
#define AC_PLATFORM_IOS
#error "IOS is not supported!"
#elif TARGET_OS_MAC == 1
#define AC_PLATFORM_MACOS
#error "MacOS is not supported!"
#else
#error "Unknown Apple platform!"
#endif
 /* We also have to check __ANDROID__ before __linux__
  * since android is based on the linux kernel
  * it has __linux__ defined */
#elif defined(__ANDROID__)
#define AC_PLATFORM_ANDROID
#error "Android is not supported!"
#elif defined(__linux__)
#define AC_PLATFORM_LINUX
#error "Linux is not supported!"
#else
	/* Unknown compiler/platform */
#error "Unknown platform!"
#endif // End of platform detection


// DLL support
#ifdef AC_PLATFORM_WINDOWS
#if AC_DYNAMIC_LINK
#ifdef AC_BUILD_DLL
#define ACRYLIC_API __declspec(dllexport)
#else
#define ACRYLIC_API __declspec(dllimport)
#endif
#else
#define ACRYLIC_API
#endif
#else
#error Acrylic only supports Windows!
#endif // End of DLL support

#ifdef AC_DEBUG
#if defined(AC_PLATFORM_WINDOWS)
#define AC_DEBUGBREAK() __debugbreak()
#elif defined(AC_PLATFORM_LINUX)
#include <signal.h>
#define AC_DEBUGBREAK() raise(SIGTRAP)
#else
#error "Platform doesn't support debugbreak yet!"
#endif
#define AC_ENABLE_ASSERTS
#else
#define AC_DEBUGBREAK()
#endif

#ifdef AC_ENABLE_ASSERTS
#define AC_ASSERT(x, ...) { if(!(x)) { AC_ERROR("Assertion Failed: {0}", __VA_ARGS__); AC_DEBUGBREAK(); } }
#define AC_CORE_ASSERT(x, ...) { if(!(x)) { AC_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); AC_DEBUGBREAK(); } }
#else
#define AC_ASSERT(x, ...)
#define AC_CORE_ASSERT(x, ...)
#endif 

#define BIT(x) (1 << x)

#define AC_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Acrylic {

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

}