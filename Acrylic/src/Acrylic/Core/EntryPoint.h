#pragma once

#ifdef AC_PLATFORM_WINDOWS

extern Acrylic::Application* Acrylic::CreateApplication();

int main(int argc, char** argv)
{
	Acrylic::Log::Init();
	AC_CORE_INFO("Initialized Log!");

	auto app = Acrylic::CreateApplication();
	app->Run();
	delete app;
}

#endif
