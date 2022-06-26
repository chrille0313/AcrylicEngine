#pragma once

#include <memory>

#include "Acrylic/Core/PlatformDetection.h"


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

#define AC_EXPAND_MACRO(x) x
#define AC_STRINGIFY_MACRO(x) #x

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

#include "Acrylic/Core/Log.h"
#include "Acrylic/Core/Assert.h"