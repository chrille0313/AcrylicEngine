#pragma once

#include "Acrylic/Core/Base.h"

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>


namespace Acrylic {

	class Log
	{
	public:
		static void Init();

		static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static Ref<spdlog::logger> s_CoreLogger;
		static Ref<spdlog::logger> s_ClientLogger;
	};

}

// Core log macros
#define AC_CORE_TRACE(...)		::Acrylic::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define AC_CORE_INFO(...)		::Acrylic::Log::GetCoreLogger()->info(__VA_ARGS__)
#define AC_CORE_WARN(...)		::Acrylic::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define AC_CORE_ERROR(...)		::Acrylic::Log::GetCoreLogger()->error(__VA_ARGS__)
#define AC_CORE_CRITICAL(...)	::Acrylic::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define AC_TRACE(...)			::Acrylic::Log::GetClientLogger()->trace(__VA_ARGS__)
#define AC_INFO(...)			::Acrylic::Log::GetClientLogger()->info(__VA_ARGS__)
#define AC_WARN(...)			::Acrylic::Log::GetClientLogger()->warn(__VA_ARGS__)
#define AC_ERROR(...)			::Acrylic::Log::GetClientLogger()->error(__VA_ARGS__)
#define AC_CRITICAL(...)		::Acrylic::Log::GetClientLogger()->critical(__VA_ARGS__)
