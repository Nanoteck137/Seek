#pragma once

#include "Seek.h"

#include "WorldController.h"
#include "MouseController.h"

class GameLayer : public Seek::Layer
{
public:
    GameLayer();

    void OnAttach() override;
    void OnDetach() override;

    void OnUpdate(Seek::Timestep ts) override;
    void OnImGuiRender(Seek::Timestep ts) override;

    virtual void OnEvent(Seek::Event& event) override;

private:
    Seek::OrthographicCamera m_Camera;
    WorldController* m_WorldController;
    MouseController* m_MouseController;
};