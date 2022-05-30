#include "EditorLayer.h"
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

namespace Acrylic {


	EditorLayer::EditorLayer() : Layer("EditorLayer"), m_MainCameraController(1280.0f / 720.0f, true)
	{
	}

	void EditorLayer::OnAttach()
	{
		AC_PROFILE_FUNCTION();

		// Textures

		m_CheckerboardTexture = Texture2D::Create("assets/textures/Checkerboard.png");

		m_SpriteSheetTiles = Texture2D::Create("Game/assets/PixelPlatformer/Sprites/Tilemap/tiles_packed.png");
		m_SpriteSheetBackgrounds = Texture2D::Create("Game/assets/PixelPlatformer/Sprites/Tilemap/tilemap_packed.png");

		m_MapWidth = s_MapWidth;
		m_MapHeight = strlen(s_MapTiles) / s_MapWidth;

		m_TextureEmpty = SubTexture2D::CreateFromCoords(m_SpriteSheetBackgrounds, { 2, 0 }, { 24, 24 });

		// Air
		m_TextureAir = SubTexture2D::CreateFromCoords(m_SpriteSheetBackgrounds, { 0, 1 }, { 24, 24 });

		// Grass
		m_TextureGrass = SubTexture2D::CreateFromCoords(m_SpriteSheetTiles, { 0, 7 }, { 18, 18 });		// Grass
		m_TextureGrass01 = SubTexture2D::CreateFromCoords(m_SpriteSheetTiles, { 1, 7 }, { 18, 18 });	// Grass01
		m_TextureGrass02 = SubTexture2D::CreateFromCoords(m_SpriteSheetTiles, { 2, 7 }, { 18, 18 });	// Grass02
		m_TextureGrass03 = SubTexture2D::CreateFromCoords(m_SpriteSheetTiles, { 3, 7 }, { 18, 18 });	// Grass03

		// Dirt
		m_TextureDirt01 = SubTexture2D::CreateFromCoords(m_SpriteSheetTiles, { 1, 2 }, { 18, 18 });
		m_TextureDirt02 = SubTexture2D::CreateFromCoords(m_SpriteSheetTiles, { 2, 2 }, { 18, 18 });
		m_TextureDirt03 = SubTexture2D::CreateFromCoords(m_SpriteSheetTiles, { 3, 2 }, { 18, 18 });


		// Map textures
		s_TextureMap['G'] = m_TextureGrass02;
		s_TextureMap['D'] = m_TextureDirt02;
		s_TextureMap['A'] = m_TextureAir;


		// Framebuffer
		FramebufferSpecification fbSpec;
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = Framebuffer::Create(fbSpec);
	}

	void EditorLayer::OnDetach()
	{
		AC_PROFILE_FUNCTION();
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		AC_PROFILE_FUNCTION();

		//AC_TRACE("{0} fps: {1}", ts, ts.GetFps());

		// Update

		m_MainCameraController.OnUpdate(ts);


		// Render
		Renderer2D::ResetStats();
		{
			AC_PROFILE_SCOPE("Renderer Prep");
			m_Framebuffer->Bind();
			RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
			RenderCommand::Clear();
		}

		// Draw
		{
			AC_PROFILE_SCOPE("Renderer Draw");

			Renderer2D::BeginScene(m_MainCameraController.GetCamera());

			//Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 0.0f, 0.0f, 0.0f }, { 5.0f, 5.0f }, m_CheckerboardTexture, { 1.0f, 1.0f, 1.0f, 1.0f }, 5.0f);

			for (uint32_t y = 0; y < m_MapHeight; y++) {
				for (uint32_t x = 0; x < m_MapWidth; x++) {
					char tileType = s_MapTiles[x + y * m_MapWidth];
					Ref<SubTexture2D> texture;

					if (s_TextureMap.find(tileType) != s_TextureMap.end()) {
						texture = s_TextureMap[tileType];
					}
					else {
						texture = m_TextureEmpty;
					}

					Renderer2D::DrawQuad({ x - s_MapWidth / 2.0f,  m_MapHeight - y - m_MapHeight / 2.0f, 0.5f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, texture);
				}
			}

			Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, m_TextureGrass02);
			//Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, m_TextureDirt01);

			Renderer2D::EndScene();

			m_Framebuffer->Unbind();
		}

	}

	void EditorLayer::OnEvent(Event& e)
	{
		m_MainCameraController.OnEvent(e);
	}

	void EditorLayer::OnImGuiRender()
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
					Application::Get().Close();

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		auto stats = Renderer2D::GetStats();

		ImGui::Begin("Settings");
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2 { 0, 0 });
		ImGui::Begin("Viewport");

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

		if (m_ViewportSize != *((glm::vec2*)&viewportPanelSize)) {
			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

			m_MainCameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
		}

		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)textureID, ImVec2 { m_ViewportSize.x, m_ViewportSize.y }, ImVec2 { 0,1 }, ImVec2 { 1,0 });

		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();

	}

}