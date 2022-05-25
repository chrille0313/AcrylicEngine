#include "acpch.h"
#include "Acrylic/Renderer/RendererAPI.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Acrylic {

	RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

	Scope<RendererAPI> RendererAPI::Create()
	{
		switch (s_API) {
			case RendererAPI::API::None:
				AC_CORE_ASSERT(false, "RendererAPI::None is currently not supporterd!");
				return nullptr;

			case RendererAPI::API::OpenGL:
				return CreateScope<OpenGLRendererAPI>();
		}

		AC_CORE_ASSERT(false, "Unkown RendererAPI");
		return nullptr;
	}
}