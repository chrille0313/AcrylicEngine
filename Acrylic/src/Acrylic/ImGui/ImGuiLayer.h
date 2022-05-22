#pragma once

#include "Acrylic/Layer.h"

#include "Acrylic/Events/ApplicationEvent.h"
#include "Acrylic/Events/KeyEvent.h"
#include "Acrylic/Events/MouseEvent.h"


namespace Acrylic {

	class ACRYLIC_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
	};

}