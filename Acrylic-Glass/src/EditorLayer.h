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
		OrthographicCameraController m_MainCameraController;

		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;
		glm::vec2 m_ViewportSize = { 0, 0 };
		Ref<Framebuffer> m_Framebuffer;


		// Textures
		Ref<Texture2D> m_CheckerboardTexture;
		Ref<Texture2D> m_SpriteSheetTiles;
		Ref<Texture2D> m_SpriteSheetBackgrounds;


		// Background
		Ref<SubTexture2D> m_TextureEmpty;
		Ref<SubTexture2D> m_TextureAir;

		// Grass
		Ref<SubTexture2D> m_TextureGrass;
		Ref<SubTexture2D> m_TextureGrass01;
		Ref<SubTexture2D> m_TextureGrass02;
		Ref<SubTexture2D> m_TextureGrass03;

		// Dirt
		Ref<SubTexture2D> m_TextureDirt01;
		Ref<SubTexture2D> m_TextureDirt02;
		Ref<SubTexture2D> m_TextureDirt03;

		uint32_t m_MapWidth, m_MapHeight;
		std::unordered_map<char, Ref<SubTexture2D>> s_TextureMap;
	};
}