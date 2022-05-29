#pragma once

#include "Acrylic.h"


class Sandbox2D : public Acrylic::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Acrylic::Timestep ts) override;
	void OnEvent(Acrylic::Event& e) override;
	virtual void OnImGuiRender() override;

private:
	Acrylic::OrthographicCameraController m_MainCameraController;

	Acrylic::Ref<Acrylic::Texture2D> m_CheckerboardTexture;
	Acrylic::Ref<Acrylic::Texture2D> m_SpriteSheetTiles;
	Acrylic::Ref<Acrylic::Texture2D> m_SpriteSheetBackgrounds;


	// Background
	Acrylic::Ref<Acrylic::SubTexture2D> m_TextureEmpty;
	Acrylic::Ref<Acrylic::SubTexture2D> m_TextureAir;

	// Grass
	Acrylic::Ref<Acrylic::SubTexture2D> m_TextureGrass;
	Acrylic::Ref<Acrylic::SubTexture2D> m_TextureGrass01;
	Acrylic::Ref<Acrylic::SubTexture2D> m_TextureGrass02;
	Acrylic::Ref<Acrylic::SubTexture2D> m_TextureGrass03;

	// Dirt
	Acrylic::Ref<Acrylic::SubTexture2D> m_TextureDirt01;
	Acrylic::Ref<Acrylic::SubTexture2D> m_TextureDirt02;
	Acrylic::Ref<Acrylic::SubTexture2D> m_TextureDirt03;

	uint32_t m_MapWidth, m_MapHeight;
	std::unordered_map<char, Acrylic::Ref<Acrylic::SubTexture2D>> s_TextureMap;
};