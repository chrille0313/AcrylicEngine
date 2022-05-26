#pragma once
#include "Acrylic/Core/Base.h"

#ifdef AC_PLATFORM_WINDOWS

extern Acrylic::Application* Acrylic::CreateApplication();

int main(int argc, char** argv)
{
	Acrylic::Log::Init();

	AC_PROFILE_BEGIN_SESSION("Startup", "AcrylicProfile-Startup.json");
	auto app = Acrylic::CreateApplication();
	AC_PROFILE_END_SESSION();

	AC_PROFILE_BEGIN_SESSION("Startup", "AcrylicProfile-Runtime.json");
	app->Run();
	AC_PROFILE_END_SESSION();

	AC_PROFILE_BEGIN_SESSION("Startup", "AcrylicProfile-Shutdown.json");
	delete app;
	AC_PROFILE_END_SESSION();
}

#endif
