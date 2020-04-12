#pragma once

#include <sstream>

#include "Seek/Events/Event.h"

namespace Seek
{
    class WindowResizeEvent : public Event
    {
    public:
        WindowResizeEvent(uint32 width, uint32 height)
            : m_Width(width), m_Height(height)
        {
        }

        inline uint32 GetWidth() const { return m_Width; }
        inline uint32 GetHeight() const { return m_Height; }

        String ToString() const override
        {
            std::stringstream ss;
            ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
            return ss.str();
        }

        EVENT_CLASS_TYPE(WindowResize)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    private:
        uint32 m_Width;
        uint32 m_Height;
    };

    class WindowCloseEvent : public Event
    {
    public:
        WindowCloseEvent() {}

        EVENT_CLASS_TYPE(WindowClose)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };
}