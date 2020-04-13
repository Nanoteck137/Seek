#pragma once

#include <Seek.h>

#include "WorldController.h"

class MouseController
{
public:
    MouseController(Seek::OrthographicCamera& camera,
                    WorldController& worldController);

    void Update(Seek::Timestep ts);

private:
    Seek::OrthographicCamera& m_Camera;
    WorldController& m_WorldController;
};