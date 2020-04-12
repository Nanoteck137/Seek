#pragma once

#include "Seek.h"

class Menu : public Seek::UIComponent
{
public:
    Menu();
    ~Menu();

protected:
    virtual void OnInit() override;
    virtual void OnUpdate(Seek::Timestep ts) override;

private:
    void AddButton(const String& label, int32 num,
                   Seek::UIButton::ActionHandler action);

private:
    Seek::Ref<Seek::Transition> m_Transition;
};