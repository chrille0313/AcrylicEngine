#include <Acrylic.h>

class TestLayer : public Acrylic::Layer
{
public:
	TestLayer() : Layer("Test") {}

	void OnUpdate() override {}

	void OnEvent(Acrylic::Event& event) override
	{
		//AC_TRACE("{0}", event);
	}
};

class Sandbox : public Acrylic::Application
{
public:
	Sandbox()
	{
		PushLayer(new TestLayer());
		PushOverlay(new Acrylic::ImGuiLayer());
	}

	~Sandbox() {}
};

Acrylic::Application* Acrylic::CreateApplication()
{
	return new Sandbox();
}