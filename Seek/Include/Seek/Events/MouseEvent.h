#pragma once

#include "Seek/Events/Event.h"

namespace Seek
{
    class MouseMovedEvent : public Event
    {
    public:
        MouseMovedEvent(float32 x, float32 y) : m_MouseX(x), m_MouseY(y) {}

        inline float32 GetX() const { return m_MouseX; }
        inline float32 GetY() const { return m_MouseY; }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MouseMovedEvent: " << m_MouseX << ", " << m_MouseY;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseMoved)
        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
    private:
        float32 m_MouseX;
        float32 m_MouseY;
    };

    class MouseScrolledEvent : public Event
    {
    public:
        MouseScrolledEvent(float32 xOffset, float32 yOffset)
            : m_XOffset(xOffset), m_YOffset(yOffset)
        {
        }

        inline float32 GetXOffset() const { return m_XOffset; }
        inline float32 GetYOffset() const { return m_YOffset; }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MouseScrolledEvent: " << GetXOffset() << ", "
               << GetYOffset();
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseScrolled)
        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
    private:
        float32 m_XOffset;
        float32 m_YOffset;
    };

    class MouseButtonEvent : public Event
    {
    public:
        inline int32 GetMouseButton() const { return m_Button; }
        inline float32 GetX() const { return m_MouseX; }
        inline float32 GetY() const { return m_MouseY; }

        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
    protected:
        MouseButtonEvent(int32 button, float32 mouseX, float32 mouseY)
            : m_Button(button), m_MouseX(mouseX), m_MouseY(mouseY)
        {
        }

        int32 m_Button;
        float32 m_MouseX;
        float32 m_MouseY;
    };

    class MouseButtonPressedEvent : public MouseButtonEvent
    {
    public:
        MouseButtonPressedEvent(int32 button, float32 mouseX, float32 mouseY)
            : MouseButtonEvent(button, mouseX, mouseY)
        {
        }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MouseButtonPressedEvent: " << m_MouseX << ", " << m_MouseY
               << " " << m_Button;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseButtonPressed)
    };

    class MouseButtonReleasedEvent : public MouseButtonEvent
    {
    public:
        MouseButtonReleasedEvent(int32 button, float32 mouseX, float32 mouseY)
            : MouseButtonEvent(button, mouseX, mouseY)
        {
        }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MouseButtonReleasedEvent: " << m_MouseX << ", " << m_MouseY
               << " " << m_Button;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseButtonReleased)
    };
}