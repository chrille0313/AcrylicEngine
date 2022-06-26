#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>


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