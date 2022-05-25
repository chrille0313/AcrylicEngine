#include "acpch.h"
#include "Acrylic/Renderer/RenderCommand.h"


namespace Acrylic {

	Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();

}