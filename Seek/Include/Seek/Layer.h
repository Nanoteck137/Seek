#pragma once

#include "Seek/Events/Event.h"
#include "Seek/Timestep.h"

namespace Seek
{
    class Layer
    {
    public:
        Layer(const std::string& name = "Layer");
        virtual ~Layer();

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate(Timestep ts) {}
        virtual void OnImGuiRender(Timestep ts) {}
        virtual void OnEvent(Event& event) {}

        inline const std::string& GetName() const { return m_DebugName; }

    private:
        std::string m_DebugName;
    };
}