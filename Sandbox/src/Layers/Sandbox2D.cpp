#include "Sandbox2D.h"
#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

static const uint32_t s_MapWidth = 24;
static const char* s_MapTiles =
"AAAAAAAAAAAAAAAAAAAAAAAA"
"AAAAAAAAAAAAAASSAAAAAAAA"
"GGGGGGGGGGGGGGGGGGGGGGGG"
"DDDDDDDDDDDDDDDDDDDDDDDD"
"DDDDDDDDDDDDDDDDDDDDDDDD"
"DDDDDDDDDDDDDDDDDDDDDDDD"
;


Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), m_MainCameraController(1280.0f / 720.0f, true)
{
}

void Sandbox2D::OnAttach()
{
	AC_PROFILE_FUNCTION();

	// Textures

	m_CheckerboardTexture = Acrylic::Texture2D::Create("assets/textures/Checkerboard.png");

	m_SpriteSheetTiles = Acrylic::Texture2D::Create("Game/assets/PixelPlatformer/Sprites/Tilemap/tiles_packed.png");
	m_SpriteSheetBackgrounds = Acrylic::Texture2D::Create("Game/assets/PixelPlatformer/Sprites/Tilemap/tilemap_packed.png");

	m_MapWidth = s_MapWidth;
	m_MapHeight = strlen(s_MapTiles) / s_MapWidth;

	m_TextureEmpty = Acrylic::SubTexture2D::CreateFromCoords(m_SpriteSheetBackgrounds, { 2, 0 }, { 24, 24 });

	// Air
	m_TextureAir = Acrylic::SubTexture2D::CreateFromCoords(m_SpriteSheetBackgrounds, { 0, 1 }, { 24, 24 });

	// Grass
	m_TextureGrass = Acrylic::SubTexture2D::CreateFromCoords(m_SpriteSheetTiles, { 0, 7 }, { 18, 18 });		// Grass
	m_TextureGrass01 = Acrylic::SubTexture2D::CreateFromCoords(m_SpriteSheetTiles, { 1, 7 }, { 18, 18 });	// Grass01
	m_TextureGrass02 = Acrylic::SubTexture2D::CreateFromCoords(m_SpriteSheetTiles, { 2, 7 }, { 18, 18 });	// Grass02
	m_TextureGrass03 = Acrylic::SubTexture2D::CreateFromCoords(m_SpriteSheetTiles, { 3, 7 }, { 18, 18 });	// Grass03

	// Dirt
	m_TextureDirt01 = Acrylic::SubTexture2D::CreateFromCoords(m_SpriteSheetTiles, { 1, 2 }, { 18, 18 });
	m_TextureDirt02 = Acrylic::SubTexture2D::CreateFromCoords(m_SpriteSheetTiles, { 2, 2 }, { 18, 18 });
	m_TextureDirt03 = Acrylic::SubTexture2D::CreateFromCoords(m_SpriteSheetTiles, { 3, 2 }, { 18, 18 });


	// Map textures
	s_TextureMap['G'] = m_TextureGrass02;
	s_TextureMap['D'] = m_TextureDirt02;
	s_TextureMap['A'] = m_TextureAir;


	// Framebuffer
	Acrylic::FramebufferSpecification fbSpec;
	fbSpec.Width = 1280;
	fbSpec.Height = 720;
	m_Framebuffer = Acrylic::Framebuffer::Create(fbSpec);
}

void Sandbox2D::OnDetach()
{
	AC_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(Acrylic::Timestep ts)
{
	AC_PROFILE_FUNCTION();

	//AC_TRACE("{0} fps: {1}", ts, ts.GetFps());

	// Update

	m_MainCameraController.OnUpdate(ts);


	// Render
	Acrylic::Renderer2D::ResetStats();
	{
		AC_PROFILE_SCOPE("Renderer Prep");
		m_Framebuffer->Bind();
		Acrylic::RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
		Acrylic::RenderCommand::Clear();
	}

	// Draw
	{
		AC_PROFILE_SCOPE("Renderer Draw");

		Acrylic::Renderer2D::BeginScene(m_MainCameraController.GetCamera());

		//Acrylic::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 0.0f, 0.0f, 0.0f }, { 5.0f, 5.0f }, m_CheckerboardTexture, { 1.0f, 1.0f, 1.0f, 1.0f }, 5.0f);

		for (uint32_t y = 0; y < m_MapHeight; y++) {
			for (uint32_t x = 0; x < m_MapWidth; x++) {
				char tileType = s_MapTiles[x + y * m_MapWidth];
				Acrylic::Ref<Acrylic::SubTexture2D> texture;

				if (s_TextureMap.find(tileType) != s_TextureMap.end()) {
					texture = s_TextureMap[tileType];
				}
				else {
					texture = m_TextureEmpty;
				}

				Acrylic::Renderer2D::DrawQuad({ x - s_MapWidth / 2.0f,  m_MapHeight - y - m_MapHeight / 2.0f, 0.5f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, texture);
			}
		}

		Acrylic::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, m_TextureGrass02);
		//Acrylic::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, m_TextureDirt01);

		Acrylic::Renderer2D::EndScene();

		m_Framebuffer->Unbind();
	}

}

void Sandbox2D::OnEvent(Acrylic::Event& e)
{
	m_MainCameraController.OnEvent(e);
}

void Sandbox2D::OnImGuiRender()
{
	AC_PROFILE_FUNCTION();

	// If you strip some features of, this demo is pretty much equivalent to calling DockSpaceOverViewport()!
	// In most cases you should be able to just call DockSpaceOverViewport() and ignore all the code below!
	// In this specific demo, we are not using DockSpaceOverViewport() because:
	// - we allow the host window to be floating/moveable instead of filling the viewport (when opt_fullscreen == false)
	// - we allow the host window to have padding (when opt_padding == true)
	// - we have a local menu bar in the host window (vs. you could use BeginMainMenuBar() + DockSpaceOverViewport() in your code!)
	// TL;DR; this demo is more complicated than what you would normally use.
	// If we removed all the options we are showcasing, this demo would become:
	//     void ShowExampleAppDockSpace()
	//     {
	//         ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
	//     }

	if (true) {
		static bool dockSpaceOpen = true;
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen) {
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else {
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockSpaceOpen, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem("Exit"))
					Acrylic::Application::Get().Close();

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		ImGui::Begin("Viewport");

		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)textureID, ImVec2 { 1280, 720 }, ImVec2 { 0,1 }, ImVec2 { 1,0 });

		ImGui::End();

		ImGui::End();
	}

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
