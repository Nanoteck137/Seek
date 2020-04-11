#include "Menu.h"

Menu::Menu() {}
Menu::~Menu() {}

void Menu::AddButton(const String& label, int num)
{
    Seek::Ref<Seek::Font> font =
        Seek::FontManager::CreateFont("Assets/Fonts/Roboto-Regular.ttf", 40.0f);

    Seek::UIButton::Properties props;
    props.Label = label;
    props.Font = font;
    props.Border = true;
    props.BorderThickness = 10.0f;

    Seek::UIButton* button = new Seek::UIButton(props);
    Seek::UIConstraints* constraints = new Seek::UIConstraints();
    constraints->SetX(new Seek::UIRelativeConstraint(0.07f));
    constraints->SetY(new Seek::UIRelativeConstraint(0.4f + num * -0.12f));
    constraints->SetWidth(new Seek::UIRatioConstraint(6.0f));
    constraints->SetHeight(new Seek::UIRelativeConstraint(0.09f));

    Add(button, constraints);
}

void Menu::OnInit()
{
    int num = 0;
    AddButton("Play", num++);
    AddButton("Settings", num++);
    AddButton("Quit", num++);
}

void Menu::OnUpdate(float deltaTime) {}
void Menu::OnDimentionsChange() {}
