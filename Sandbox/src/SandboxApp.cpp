#include <Acrylic.h>


class TestLayer : public Acrylic::Layer
{
public:
	TestLayer() : Layer("Test") {}

	void OnUpdate() override
	{
		AC_INFO("ExampleLayer::Update");
	}

	void OnEvent(Acrylic::Event& event) override
	{
		AC_TRACE("{0}", event);
	}
};

class Sandbox : public Acrylic::Application
{
public:
	Sandbox()
	{
		PushLayer(new TestLayer());
	}

	~Sandbox() {}
};

Acrylic::Application* Acrylic::CreateApplication()
{
	return new Sandbox();
}