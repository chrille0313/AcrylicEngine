#pragma once

#include "Acrylic.h"
#include "Panels/ViewportPanel.h"
#include "Panels/SceneHierarchyPanel.h"


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
		OrthographicCameraController m_EditorCameraController;


		// Scene
		Ref<Scene> m_ActiveScene;


		// Entities
		Entity m_MainCamera;


		// Textures
		Ref<Texture2D> m_CheckerboardTexture;


		// Panels
		ViewportPanel m_ViewportPanel;
		SceneHierarchyPanel m_SceneHierarchyPanel;
	};

}