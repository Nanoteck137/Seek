#pragma once

#include "Seek/Core.h"
#include "Seek/Log.h"

#include "Seek/Events/Event.h"
#include "Seek/App/Timestep.h"

namespace Seek
{
    class Layer
    {
    public:
        Layer(const String& name = "Layer");
        virtual ~Layer();

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate(Timestep ts) {}
        virtual void OnImGuiRender(Timestep ts) {}
        virtual void OnEvent(Event& event) {}

        inline const String& GetName() const { return m_DebugName; }

    private:
        String m_DebugName;
    };
}