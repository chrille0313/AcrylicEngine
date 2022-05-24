#include "Sandbox2D.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), m_MainCameraController(1280.0f / 720.0f, true)
{
}

void Sandbox2D::OnAttach()
{
	m_CheckerboardTexture = Acrylic::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Acrylic::Timestep ts)
{
	//AC_TRACE("Delta time: {0}s ({1}ms)", ts.GetSeconds(), ts.GetMilliseconds());

	// Update
	m_MainCameraController.OnUpdate(ts);

	// Render
	Acrylic::RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1 });
	Acrylic::RenderCommand::Clear();



	Acrylic::Renderer2D::BeginScene(m_MainCameraController.GetCamera());

	Acrylic::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
	Acrylic::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.0f, 0.0f, 0.0f }, { 0.5f, 1.0f }, { 0.3f, 0.5f, 0.6f, 1.0f });
	Acrylic::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 0.0f, 0.0f, 0.0f }, { 5.0f, 5.0f }, m_CheckerboardTexture, { 0.1f, 0.3f, 0.7f, 1.0f });

	Acrylic::Renderer2D::EndScene();
}

void Sandbox2D::OnEvent(Acrylic::Event& e)
{
	m_MainCameraController.OnEvent(e);
}

void Sandbox2D::OnImGuiRender()
{
	//ImGui::Begin("Settings");
	//ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	//ImGui::End();
}
