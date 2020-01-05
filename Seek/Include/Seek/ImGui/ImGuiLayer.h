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

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnImGuiRender() override;

        void Begin();
        void End();

    private:
        float m_Time = 0.0f;
    };

}