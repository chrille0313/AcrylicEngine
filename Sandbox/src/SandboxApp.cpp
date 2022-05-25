#include <Acrylic.h>
#include <Acrylic/Core/EntryPoint.h>

#include "Layers/Sandbox2D.h"
#include "Layers/TestLayer.h"


class Sandbox : public Acrylic::Application
{
public:
	Sandbox()
	{
		//PushLayer(new TestLayer());
		PushLayer(new Sandbox2D);
	}

	~Sandbox()
	{
	}
};

Acrylic::Application* Acrylic::CreateApplication()
{
	return new Sandbox();
}