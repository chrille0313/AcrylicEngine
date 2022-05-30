#pragma once

#include "Acrylic/Core/Base.h"


namespace Acrylic {

	struct FramebufferSpecification
	{
		uint32_t Width, Height;
		uint32_t Samples = 1;

		bool SwapChainTarget = false;
	};

	class Framebuffer
	{
	public:
		static Ref<Framebuffer>Create(const FramebufferSpecification& spec);
		virtual ~Framebuffer() = default;

		virtual const FramebufferSpecification& GetSpecification() const = 0;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual uint32_t GetColorAttachmentRendererID() const = 0;
	};

}