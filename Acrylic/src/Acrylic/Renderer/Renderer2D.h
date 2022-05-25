#pragma once

#include "Acrylic/Renderer/OrthographicCamera.h"

#include "Acrylic/Renderer/Texture.h"


namespace Acrylic {

	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		// Primitives
		static void DrawQuad(const glm::vec2& position, const glm::vec3& rotation, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec3& rotation, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec3& rotation, const glm::vec2& size, const Ref<Texture2D> texture, const glm::vec4& tint = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& position, const glm::vec3& rotation, const glm::vec2& size, const Ref<Texture2D> texture, const glm::vec4& tint = glm::vec4(1.0f));
	};

}