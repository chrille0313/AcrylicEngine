#include "acpch.h"
#include "Acrylic/Renderer/Framebuffer.h"

#include "Acrylic/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLFramebuffer.h"


namespace Acrylic {

	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
	{
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:
				AC_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
				return nullptr;
			case RendererAPI::API::OpenGL:
				return CreateRef<OpenGLFramebuffer>(spec);
		}

		AC_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}