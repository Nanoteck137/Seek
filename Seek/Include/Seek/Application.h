#pragma once

#include "Seek/Window.h"

#include "Seek/Events/ApplicationEvent.h"
#include "Seek/LayerStack.h"

namespace Seek
{
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		static inline Application& Get() { return *s_Instance; }

		inline Window& GetWindow() { return *m_Window; }
	private:
		bool OnWindowClosed(WindowCloseEvent& e);
	private:
		bool m_Running = false;

		std::unique_ptr<Window> m_Window;

		LayerStack m_LayerStack;
	private:
		static Application* s_Instance;
	};
}