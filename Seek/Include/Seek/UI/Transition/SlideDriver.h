#pragma once

#include "Seek/Core.h"
#include "Seek/Log.h"

#include "ValueDriver.h"

namespace Seek
{
    class SlideDriver : public ValueDriver
    {
    public:
        SlideDriver(float start, float end, float length);
        SlideDriver(float start, float end, float length, float timeDelay);

    protected:
        virtual float CalculateValue(float time) override;

    private:
        float m_StartValue;
        float m_EndValue;
    };
}