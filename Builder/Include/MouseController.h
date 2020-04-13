#pragma once

#include <Seek.h>

#include "WorldController.h"

class MouseController
{
public:
    MouseController(Seek::OrthographicCamera& camera,
                    WorldController& worldController);

    void Update(Seek::Timestep ts);
    void OnEvent(Seek::Event& event);

private:
    bool OnMouseButtonReleased(Seek::MouseButtonReleasedEvent& event);

private:
    Seek::OrthographicCamera& m_Camera;
    WorldController& m_WorldController;
};