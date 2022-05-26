#include "acpch.h"
#include "Acrylic/Renderer/Buffer.h"

#include "Acrylic/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

/*
#if AC_PLATFORM_WINDOWS
#include "Platform/Direct3D/Direct3DBuffer.h"
#endif
*/

namespace Acrylic {

	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:
				AC_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
				return nullptr;
				break;
			case RendererAPI::API::OpenGL:
				return CreateRef<OpenGLVertexBuffer>(size);
		}

		AC_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:
				AC_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
				return nullptr;
				break;
			case RendererAPI::API::OpenGL:
				return CreateRef<OpenGLVertexBuffer>(vertices, size);
		}

		AC_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:
				AC_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
				return nullptr;
				break;
			case RendererAPI::API::OpenGL:
				return CreateRef<OpenGLIndexBuffer>(indices, size);
		}

		AC_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}