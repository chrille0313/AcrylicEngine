#include "acpch.h"
#include "Acrylic/Renderer/Texture.h"

#include "Acrylic/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"


namespace Acrylic {

	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:
				AC_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
				return nullptr;
			case RendererAPI::API::OpenGL:
				return CreateRef<OpenGLTexture2D>(width, height);
		}

		AC_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:
				AC_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
				return nullptr;
			case RendererAPI::API::OpenGL:
				return CreateRef<OpenGLTexture2D>(path);
		}

		AC_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}