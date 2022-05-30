#pragma once

#include "Acrylic/Core/Layer.h"

#include "Acrylic/Events/ApplicationEvent.h"
#include "Acrylic/Events/KeyEvent.h"
#include "Acrylic/Events/MouseEvent.h"


namespace Acrylic {

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& e) override;

		void Begin();
		void End();

		void BlockEvents(bool block) { m_BlockEvents = block; }

	private:
		bool m_BlockEvents = false;
		float m_Time = 0.0f;
	};

}