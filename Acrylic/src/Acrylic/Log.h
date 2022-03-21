#pragma once

#include <memory>

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"


namespace Acrylic {

	class ACRYLIC_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}

// Core log macros
#define AC_CORE_TRACE(...)		::Acrylic::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define AC_CORE_INFO(...)		::Acrylic::Log::GetCoreLogger()->info(__VA_ARGS__)
#define AC_CORE_WARN(...)		::Acrylic::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define AC_CORE_ERROR(...)		::Acrylic::Log::GetCoreLogger()->error(__VA_ARGS__)
#define AC_CORE_FATAL(...)		::Acrylic::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Client log macros
#define AC_CLIENT_TRACE(...)	::Acrylic::Log::GetClientLogger()->trace(__VA_ARGS__)
#define AC_CLIENT_INFO(...)		::Acrylic::Log::GetClientLogger()->info(__VA_ARGS__)
#define AC_CLIENT_WARN(...)		::Acrylic::Log::GetClientLogger()->warn(__VA_ARGS__)
#define AC_CLIENT_ERROR(...)	::Acrylic::Log::GetClientLogger()->error(__VA_ARGS__)
#define AC_CLIENT_FATAL(...)	::Acrylic::Log::GetClientLogger()->fatal(__VA_ARGS__)
