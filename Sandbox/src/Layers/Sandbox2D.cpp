#include "Sandbox2D.h"
#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

static const uint32_t s_MapWidth = 24;
static const char* s_MapTiles =
"AAAAAAAAAAAAAAAAAAAAAAAA"
"AAAAAAAAAAAAAASSAAAAAAAA"
"AAAAAAAAAAAAAAAAAAAAAAAA"
"GGGGGGGGGGGGGGGGGGGGGGGG"
"DDDDDDDDDDDDDDDDDDDDDDDD"
"DDDDDDDDDDDDDDDDDDDDDDDD"
;


Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), m_MainCameraController(1280.0f / 720.0f, true)
{
}

void Sandbox2D::OnAttach()
{
	AC_PROFILE_FUNCTION();

	m_CheckerboardTexture = Acrylic::Texture2D::Create("assets/textures/Checkerboard.png");

	// PixelPlatformer
	m_SpriteSheetTiles = Acrylic::Texture2D::Create("Game/assets/PixelPlatformer/Sprites/Tilemap/tiles_packed.png");
	m_SpriteSheetBackgrounds = Acrylic::Texture2D::Create("Game/assets/PixelPlatformer/Sprites/Tilemap/tilemap_packed.png");

	m_MapWidth = s_MapWidth;
	m_MapHeight = strlen(s_MapTiles) / s_MapWidth;

	m_TextureEmpty = Acrylic::SubTexture2D::CreateFromCoords(m_SpriteSheetBackgrounds, { 2, 0 }, { 24, 24 });

	// Air
	m_TextureAir = Acrylic::SubTexture2D::CreateFromCoords(m_SpriteSheetBackgrounds, { 0, 1 }, { 24, 24 });

	// Texture
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
}

void Sandbox2D::OnDetach()
{
	AC_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(Acrylic::Timestep ts)
{
	AC_PROFILE_FUNCTION();

	AC_TRACE("{0} fps: {1}", ts, ts.GetFps());

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
