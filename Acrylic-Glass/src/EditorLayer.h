#pragma once

#include "Acrylic.h"


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
		OrthographicCameraController m_EditorCameraController;

		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;
		glm::vec2 m_ViewportSize = { 0, 0 };
		Ref<Framebuffer> m_Framebuffer;


		// Scene
		Ref<Scene> m_ActiveScene;


		// Entities
		Entity m_MainCamera;


		// Textures
		Ref<Texture2D> m_CheckerboardTexture;
	};
}