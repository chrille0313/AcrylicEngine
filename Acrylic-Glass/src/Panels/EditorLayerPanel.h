#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Acrylic/Core/Base.h"
#include "Acrylic/Scene/Scene.h"
#include "Acrylic/Scene/Entity.h"
#include "Acrylic/Scene/Components.h"

#include "Acrylic/Renderer/Framebuffer.h"


class EditorLayerPanel
{
public:
	void OnImGuiRender(const char* name, ImGuiWindowFlags flags)
	{
		ImGui::Begin(name, NULL, flags);

		OnImGuiRenderInternal();

		ImGui::End();
	}

protected:
	virtual void OnImGuiRenderInternal() {};

};