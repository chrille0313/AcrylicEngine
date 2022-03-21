#pragma once

#ifdef AC_PLATFORM_WINDOWS

extern Acrylic::Application* Acrylic::CreateApplication();

int main(int argc, char** argv)
{
	Acrylic::Log::Init();
	AC_CORE_WARN("Initialized Log!");
	int a = 5;
	AC_CLIENT_INFO("DIN MAMMA={0}", a);


	auto app = Acrylic::CreateApplication();
	app->Run();
	delete app;
}

#endif
