#include "Menu.h"

Menu::Menu() {}
Menu::~Menu() {}

void Menu::OnInit()
{
    m_Transition = Seek::CreateRef<Seek::Transition>();
    m_Transition->Add(Seek::TransitionType::XPOS,
                      new Seek::SlideTransition(-1.5f, 0.4f));
    m_Transition->Add(Seek::TransitionType::ALPHA,
                      new Seek::SlideTransition(0.0f, 0.4f));

    int num = 0;
    AddButton("Play", num++, [=]() { Display(false); });
    AddButton("Settings", num++, []() { SK_APP_INFO("Open Settings"); });
    AddButton("Quit", num++, []() { Seek::Application::Get().Close(); });
}

void Menu::OnUpdate(Seek::Timestep ts) {}

void Menu::AddButton(const String& label, int32 num,
                     Seek::UIButton::ActionHandler action)
{
    Seek::Ref<Seek::Font> font =
        Seek::FontManager::CreateFont("Assets/Fonts/Roboto-Regular.ttf", 40.0f);

    Seek::UIButton::Properties props;
    props.Label = label;
    props.Font = font;
    props.Border = true;
    props.BorderThickness = 10.0f;
    props.Color = glm::vec4(1.0f, 1.0f, 1.0f, 0.8f);

    Seek::UIButton* button = new Seek::UIButton(props);
    button->SetAction(action);
    Seek::UIConstraints* constraints = new Seek::UIConstraints();
    constraints->SetX(new Seek::UIRelativeConstraint(0.07f));
    constraints->SetY(new Seek::UIRelativeConstraint(0.4f + num * -0.12f));
    constraints->SetWidth(new Seek::UIRatioConstraint(6.0f));
    constraints->SetHeight(new Seek::UIRelativeConstraint(0.09f));

    Add(button, constraints);

    float delay = num * 0.1f;
    button->SetDisplayTransition(m_Transition, delay, delay);
}
