#include <Acrylic.h>


class TestLayer : public Acrylic::Layer {
public:
	TestLayer() : Layer("Test") {}

	void OnUpdate() override
	{
		if (Acrylic::Input::IsKeyPressed(AC_KEY_SPACE))
			AC_TRACE("Space key is pressed!");
	}

	void OnEvent(Acrylic::Event& event) override
	{
		if (event.GetEventType() == Acrylic::EventType::KeyPressed) {
			Acrylic::KeyPressedEvent& e = (Acrylic::KeyPressedEvent&)event;
			AC_TRACE("{0}", (char)e.GetKeyCode());
		}
	}
};


class Sandbox : public Acrylic::Application {
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