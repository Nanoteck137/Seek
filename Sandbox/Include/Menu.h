#pragma once

#include "Seek.h"

class Menu : public Seek::UIComponent
{
public:
    Menu();
    ~Menu();

private:
    void AddButton(const String& label, int num,
                   Seek::UIButton::ActionHandler action);

protected:
    virtual void OnInit() override;
    virtual void OnUpdate(float deltaTime) override;
    virtual void OnDimentionsChange() override;

private:
    Seek::Ref<Seek::Transition> m_Transition;
};