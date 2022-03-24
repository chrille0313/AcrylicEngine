#include "acpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"


namespace Acrylic {

	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;

}