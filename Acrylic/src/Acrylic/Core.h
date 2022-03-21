#pragma once


#ifdef AC_PLATFORM_WINDOWS
#ifdef AC_BUILD_DLL
#define ACRYLIC_API __declspec(dllexport)
#else
#define ACRYLIC_API __declspec(dllimport)
#endif
#else
#error Acrylic only supports Windows!
#endif

#ifdef AC_ENABLE_ASSERTS
#define AC_ASSERT(x, ...) { if(!(x)) { AC_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#define AC_CORE_ASSERT(x, ...) { if(!(x)) { AC_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
#define AC_ASSERT(x, ...)
#define AC_CORE_ASSERT(x, ...)
#endif 
#define BIT(x) (1 << x)