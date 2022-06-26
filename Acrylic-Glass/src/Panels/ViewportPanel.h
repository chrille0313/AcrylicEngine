#pragma once

#include "EditorLayerPanel.h"

#include "Acrylic/Core/Base.h"
#include "Acrylic/Core/Log.h"
#include "Acrylic/Scene/Entity.h"

#include "Acrylic/Renderer/Framebuffer.h"

#include <glm/glm.hpp>


namespace Acrylic {

	class ViewportPanel : public EditorLayerPanel
	{
	public:
		ViewportPanel() = default;
		virtual ~ViewportPanel() = default;

		void SetFramebuffer(FramebufferSpecification& fbSpec);

		Ref<Framebuffer> GetFramebuffer() { return m_Framebuffer; }

		bool IsHovered() { return m_ViewportHovered; }
		bool IsFocused() { return m_ViewportFocused; }

	protected:
		void OnImGuiRenderInternal() override;

	private:
		friend class EditorLayer;

		Ref<Framebuffer> m_Framebuffer;
		glm::vec2 m_ViewportSize = { 0, 0 };

		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;
	};

}