#pragma once

#include <Seek.h>

#include "WorldController.h"

class MouseController
{
public:
    MouseController(WorldController& worldController);

    void Update(Seek::Timestep ts);

private:
    WorldController& m_WorldController;
};