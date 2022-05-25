#include "acpch.h"
#include "VertexArray.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"


namespace Acrylic {

	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:
				AC_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
				return nullptr;
			case RendererAPI::API::OpenGL:
				return CreateRef<OpenGLVertexArray>();
		}

		AC_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}



}