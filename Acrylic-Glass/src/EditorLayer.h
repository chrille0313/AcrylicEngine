#pragma once

#include "Acrylic.h"
#include "Panels/SceneHierarchyPanel.h"

#include "Acrylic/Renderer/EditorCamera.h"


namespace Acrylic {

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(Timestep ts) override;
		void OnEvent(Event& e) override;
		virtual void OnImGuiRender() override;

	private:
		bool OnKeyPressed(KeyPressedEvent& e);

		void NewScene();
		void OpenScene();
		void SaveSceneAs();

	private:
		int m_GizmoType = 0;


		// Scene
		Ref<Scene> m_ActiveScene;


		// Entities
		Entity m_MainCamera;


		// Textures
		Ref<Texture2D> m_CheckerboardTexture;


		// Editor
		EditorCamera m_EditorCamera;

		// Viewport Panel
		Ref<Framebuffer> m_Framebuffer;
		glm::vec2 m_ViewportSize = { 0, 0 };

		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;

		// SceneHierarchy Panel
		SceneHierarchyPanel m_SceneHierarchyPanel;
	};

}