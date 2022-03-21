#include "Application.h"

#include "Acrylic/Events/ApplicationEvent.h"
#include "Acrylic/log.h"


namespace Acrylic {

	Application::Application()
	{

	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		WindowResizeEvent e(1280, 720);
		AC_CORE_TRACE(e);

		while (true);
	}

}
