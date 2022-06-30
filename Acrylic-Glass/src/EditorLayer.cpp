#include "EditorLayer.h"

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Acrylic/Scene/SceneSerializer.h"

#include "Acrylic/Utils/PlatformUtils.h"


namespace Acrylic {

	EditorLayer::EditorLayer() : Layer("EditorLayer"), m_EditorCameraController(1280.0f / 720.0f, true)
	{
	}

	void EditorLayer::OnAttach()
	{
		AC_PROFILE_FUNCTION();

		// Textures
		m_CheckerboardTexture = Texture2D::Create("assets/textures/Checkerboard.png");


		// Scene
		m_ActiveScene = CreateRef<Scene>();


		// Entities
		Entity entity = m_ActiveScene->CreateEntity("Square Entity");
		entity.AddComponent<SpriteRendererComponent>(glm::vec4 { 0.0f, 1.0f, 0.0f, 1.0f });

		m_MainCamera = m_ActiveScene->CreateEntity("MainCamera");
		m_MainCamera.AddComponent<CameraComponent>();
		m_MainCamera.GetComponent<CameraComponent>().Primary = true;

		/*
		class CameraController : public ScriptableEntity
		{
		public:
			void OnCreate()
			{

			}

			void OnDestroy()
			{

			}

			void OnUpdate(Timestep ts)
			{

			}
		};

		m_MainCamera.AddComponent <NativeScriptComponent>().Bind<CameraController>();
		*/

		FramebufferSpecification fbSpec;
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_ViewportPanel.SetFramebuffer(fbSpec);
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::OnDetach()
	{
		AC_PROFILE_FUNCTION();
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		AC_PROFILE_FUNCTION();

		//AC_TRACE("{0} fps: {1}", ts, ts.GetFps());


		// Resize Framebuffer
		if (Acrylic::FramebufferSpecification spec = m_ViewportPanel.GetFramebuffer()->GetSpecification();
			m_ViewportPanel.m_ViewportSize.x > 0.0f && m_ViewportPanel.m_ViewportSize.y > 0.0f && (spec.Width != m_ViewportPanel.m_ViewportSize.x || spec.Height != m_ViewportPanel.m_ViewportSize.y)) {

			m_ViewportPanel.GetFramebuffer()->Resize((uint32_t)m_ViewportPanel.m_ViewportSize.x, (uint32_t)m_ViewportPanel.m_ViewportSize.y);
			m_EditorCameraController.OnResize(m_ViewportPanel.m_ViewportSize.x, m_ViewportPanel.m_ViewportSize.y);
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportPanel.m_ViewportSize.x, (uint32_t)m_ViewportPanel.m_ViewportSize.y);
		}



		// Update
		if (m_ViewportPanel.IsFocused())
			m_EditorCameraController.OnUpdate(ts);


		// Render Prep
		Renderer2D::ResetStats();
		{
			AC_PROFILE_SCOPE("Render Prep");

			m_ViewportPanel.m_Framebuffer->Bind();
			RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
			RenderCommand::Clear();
		}


		// Render Scene
		{
			AC_PROFILE_SCOPE("Render Scene");
			m_ActiveScene->OnUpdate(ts);
		}

		m_ViewportPanel.m_Framebuffer->Unbind();
	}

	void EditorLayer::OnEvent(Event& e)
	{
		m_EditorCameraController.OnEvent(e);

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(AC_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
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


		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();

		float minWindowWidth = style.WindowMinSize.x;
		style.WindowMinSize.x = 350.0f;

		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		style.WindowMinSize.x = minWindowWidth;


		// TOP MENU BAR

		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("File")) {

				if (ImGui::MenuItem("New", "Ctrl+N"))
					NewScene();

				if (ImGui::MenuItem("Open...", "Ctrl+O"))
					OpenScene();

				ImGui::Separator();

				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
					SaveSceneAs();

				ImGui::Separator();

				if (ImGui::MenuItem("Exit"))
					Application::Get().Close();

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		// END TOP MENU BAR


		// PANELS
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoCollapse;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2 { 0, 0 });
		m_ViewportPanel.OnImGuiRender("Viewport", windowFlags);
		ImGui::PopStyleVar();

		m_SceneHierarchyPanel.OnImGuiRender("SceneHierarchy", windowFlags);

		// END PANELS


		Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportPanel.IsFocused() || !m_ViewportPanel.IsHovered());


		// DEBUG

		auto stats = Renderer2D::GetStats();
		ImGui::Begin("Debug");
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
		ImGui::End();


		// END DEBUG


		ImGui::End();  // End Dockspace

		//ImGui::ShowDemoWindow();
		//ImGui::ShowMetricsWindow();
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		if (e.GetRepeatCount() > 0)
			return false;

		bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
		bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);

		switch (e.GetKeyCode()) {
			case Key::N:
				if (control)
					NewScene();

				break;

			case Key::O:
				if (control)
					OpenScene();

				break;

			case Key::S:
				if (control && shift)
					SaveSceneAs();

				break;
		}
	}

	void EditorLayer::NewScene()
	{
		m_ActiveScene = CreateRef<Scene>();
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportPanel.m_ViewportSize.x, (uint32_t)m_ViewportPanel.m_ViewportSize.y);
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::OpenScene()
	{
		std::optional<std::string> filePath = FileDialogs::OpenFile("Acrylic Scene (*.acrylic)\0*.acrylic\0");

		if (filePath) {
			m_ActiveScene = CreateRef<Scene>();
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportPanel.m_ViewportSize.x, (uint32_t)m_ViewportPanel.m_ViewportSize.y);
			m_SceneHierarchyPanel.SetContext(m_ActiveScene);

			SceneSerializer serializer(m_ActiveScene);
			serializer.Deserialize(*filePath);
		}
	}

	void EditorLayer::SaveSceneAs()
	{
		std::optional<std::string> filePath = FileDialogs::SaveFile("Acrylic Svene (*.acrylic)\0*.acrylic\0");

		if (filePath) {
			SceneSerializer serializer(m_ActiveScene);
			serializer.Deserialize(*filePath);
		}
	}

}