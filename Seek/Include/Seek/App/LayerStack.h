#pragma once

#include "Seek/Core.h"
#include "Seek/Log.h"

#include "Seek/App/Layer.h"

namespace Seek
{
    class LayerStack
    {
    public:
        LayerStack();
        ~LayerStack();

    public:
        // NOTE: Normal Methods
        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);

        void PopLayer(Layer* layer);
        void PopOverlay(Layer* overlay);

        std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
        std::vector<Layer*>::iterator end() { return m_Layers.end(); }

    private:
        // NOTE: Normal Variables
        std::vector<Layer*> m_Layers;
        unsigned int m_LayerInsertIndex = 0;
    };
}