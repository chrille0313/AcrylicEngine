#include "Sandbox2D.h"
#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"


Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), m_MainCameraController(1280.0f / 720.0f, true)
{
}

void Sandbox2D::OnAttach()
{
	AC_PROFILE_FUNCTION();

	m_CheckerboardTexture = Acrylic::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox2D::OnDetach()
{
	AC_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(Acrylic::Timestep ts)
{
	AC_PROFILE_FUNCTION();

	// Update

	m_MainCameraController.OnUpdate(ts);


	// Render
	Acrylic::Renderer2D::ResetStats();
	{
		AC_PROFILE_SCOPE("Renderer Prep");

		Acrylic::RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
		Acrylic::RenderCommand::Clear();
	}

	// Draw
	{
		AC_PROFILE_SCOPE("Renderer Draw");

		Acrylic::Renderer2D::BeginScene(m_MainCameraController.GetCamera());

		Acrylic::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		Acrylic::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.0f, 0.0f, 45.0f }, { 0.5f, 1.0f }, { 0.3f, 0.5f, 0.6f, 1.0f });
		Acrylic::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 0.0f, 0.0f, 0.0f }, { 5.0f, 5.0f }, m_CheckerboardTexture, { 1.0f, 1.0f, 1.0f, 1.0f }, 5.0f);

		//Acrylic::Renderer2D::EndScene();

		//Acrylic::Renderer2D::BeginScene(m_MainCameraController.GetCamera());

		for (float y = -5.0f; y < 5.0f; y += 0.1f) {
			for (float x = -5.0f; x < 5.0f; x += 0.1f) {
				glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.75f };
				Acrylic::Renderer2D::DrawQuad({ x, y }, glm::vec3(0.0f), { 0.45f, 0.45f }, color);
			}
		}

		Acrylic::Renderer2D::EndScene();
	}

}

void Sandbox2D::OnEvent(Acrylic::Event& e)
{
	m_MainCameraController.OnEvent(e);
}

void Sandbox2D::OnImGuiRender()
{
	AC_PROFILE_FUNCTION();

	auto stats = Acrylic::Renderer2D::GetStats();

	ImGui::Begin("Settings");
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

	//ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}
