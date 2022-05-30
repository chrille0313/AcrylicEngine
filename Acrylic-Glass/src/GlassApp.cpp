#include <Acrylic.h>
#include <Acrylic/Core/EntryPoint.h>

#include "EditorLayer.h"


namespace Acrylic {

	class Glass : public Application
	{
	public:
		Glass() : Application("Glass Editor")
		{
			PushLayer(new EditorLayer());
		}

		~Glass()
		{
		}
	};

	Application* CreateApplication()
	{
		return new Glass();
	}

}
