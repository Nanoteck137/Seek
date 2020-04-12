#pragma once

#include "Seek/Core.h"
#include "Seek/Log.h"

#include "Seek/Events/Event.h"

namespace Seek
{
    class Window
    {
    public:
        struct Properties
        {
            String Title;
            unsigned int Width;
            unsigned int Height;

            Properties(const std::string& title = "Seek Engine",
                       unsigned int width = 1280, unsigned int height = 720)
                : Title(title), Width(width), Height(height)
            {
            }
        };

        using EventCallbackFn = std::function<void(Event&)>;

    protected:
        Window() {}

    public:
        virtual ~Window() {}

        virtual void OnUpdate() = 0;

        virtual unsigned int GetWidth() = 0;
        virtual unsigned int GetHeight() = 0;

        virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
        virtual void SetVSync(bool enabled) = 0;
        virtual bool GetVSync() const = 0;

        virtual void* GetNativeWindow() const = 0;

        static Window* Create(const Properties& props = Properties());
    };
}