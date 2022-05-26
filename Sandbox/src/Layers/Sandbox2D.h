#pragma once

#include "Acrylic.h"


class Sandbox2D : public Acrylic::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Acrylic::Timestep ts) override;
	void OnEvent(Acrylic::Event& e) override;
	virtual void OnImGuiRender() override;

private:
	Acrylic::OrthographicCameraController m_MainCameraController;


	Acrylic::Ref<Acrylic::Texture2D> m_CheckerboardTexture;

	//glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
};