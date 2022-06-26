#include "ViewportPanel.h"


namespace Acrylic {

	void ViewportPanel::SetFramebuffer(FramebufferSpecification& fbSpec)
	{
		m_Framebuffer = Framebuffer::Create(fbSpec);
	}

	void ViewportPanel::OnImGuiRenderInternal()
	{
		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		uint64_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2 { m_ViewportSize.x, m_ViewportSize.y }, ImVec2 { 0,1 }, ImVec2 { 1,0 });
	}

}