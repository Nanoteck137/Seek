#pragma once

#include "Seek/Layer.h"

#include "Seek/Events/ApplicationEvent.h"
#include "Seek/Events/KeyEvents.h"
#include "Seek/Events/MouseEvent.h"

namespace Seek
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate() override;
		void OnEvent(Event& event) override;
	private:
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
		bool OnMouseButtonReleased(MouseButtonReleasedEvent& e);
		bool OnMouseMoved(MouseMovedEvent& e);
		bool OnMouseScrolled(MouseScrolledEvent& e);

		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnKeyReleased(KeyReleasedEvent& e);
		bool OnKeyTyped(KeyTypedEvent& e);

		bool OnWindowResize(WindowResizeEvent& e);
	private:
		float m_Time = 0.0f;
	};

}