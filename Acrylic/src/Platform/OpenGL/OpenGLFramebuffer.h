#pragma once

#include "Acrylic/Renderer/Framebuffer.h"


namespace Acrylic {

	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& spec);
		virtual ~OpenGLFramebuffer();

		void Resize();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual const FramebufferSpecification& GetSpecification() const override { return m_Specification; };

		virtual uint32_t GetColorAttachmentRendererID() const override { return m_ColorAttachment; }

	private:
		uint32_t m_RendererID;
		uint32_t m_ColorAttachment;
		uint32_t m_DepthAttachment;
		FramebufferSpecification m_Specification;
	};

}