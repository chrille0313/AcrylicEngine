#include <Acrylic.h>

class Sandbox : public Acrylic::Application
{
public:
	Sandbox()
	{

	}

	~Sandbox()
	{

	}
};

Acrylic::Application* Acrylic::CreateApplication()
{
	return new Sandbox();
}
