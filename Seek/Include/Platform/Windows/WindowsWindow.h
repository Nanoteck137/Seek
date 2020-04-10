#pragma once

#include "Seek/Window.h"

#include "Seek/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Seek
{
    class WindowsWindow : public Window
    {
    public:
        WindowsWindow(const WindowProps& props);
        virtual ~WindowsWindow();

        void OnUpdate() override;

        inline uint32 GetWidth() override { return m_Data.Width; }
        inline uint32 GetHeight() override { return m_Data.Height; }

        void SetEventCallback(const EventCallbackFn& callback) override
        {
            m_Data.EventCallback = callback;
        }

        void SetVSync(bool enabled) override;
        bool GetVSync() const override;

        inline void* GetNativeWindow() const override { return m_Window; }

    private:
        virtual void Init(const WindowProps& props);
        virtual void Shutdown();

    private:
        GLFWwindow* m_Window = nullptr;
        GraphicsContext* m_Context = nullptr;

        struct WindowData
        {
            std::string Title;
            unsigned int Width, Height;
            bool VSync;

            EventCallbackFn EventCallback;
        };

        WindowData m_Data;
    };

}