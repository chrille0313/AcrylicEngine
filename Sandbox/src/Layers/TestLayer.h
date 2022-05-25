#pragma once

#include "Acrylic.h"


class TestLayer : public Acrylic::Layer
{
public:
	TestLayer();
	virtual ~TestLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Acrylic::Timestep ts) override;
	void OnEvent(Acrylic::Event& e) override;
	virtual void OnImGuiRender() override;

	bool OnKeyPressed(Acrylic::KeyPressedEvent& e)
	{
		//AC_TRACE("{0}", glm::to_string(m_MainCamera.GetPosition()));
		return false;
	}

private:
	Acrylic::ShaderLibrary m_ShaderLibrary;

	Acrylic::Ref<Acrylic::Shader> m_TriangleShader;
	Acrylic::Ref<Acrylic::VertexArray> m_TriangleVertexArray;

	Acrylic::Ref<Acrylic::Shader> m_FlatColorShader;
	Acrylic::Ref<Acrylic::VertexArray> m_SquareVertexArray;

	Acrylic::Ref<Acrylic::Texture2D> m_Texture;
	Acrylic::Ref<Acrylic::Texture2D> m_TransparentTexture;

	Acrylic::OrthographicCameraController m_MainCameraController;

	glm::vec3 m_SquareColor = { 0.2, 0.3f, 0.8f };
};