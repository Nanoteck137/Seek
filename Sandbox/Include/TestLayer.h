#pragma once

#include <Seek.h>
#include <imgui.h>
#include <box2d/box2d.h>

#include "Menu.h"

class TestLayer : public Seek::Layer
{
public:
    TestLayer();

    void OnAttach() override;
    void OnDetach() override;

    void OnUpdate(Seek::Timestep ts) override;
    void OnImGuiRender(Seek::Timestep ts) override;

    virtual void OnEvent(Seek::Event& event) override;

private:
    bool OnWindowResize(Seek::WindowResizeEvent& e);

private:
    Seek::OrthographicCamera m_Camera;
    Seek::Ref<Seek::Texture2D> m_Texture;

    Seek::Ref<Seek::Sound> m_Sound;

    Seek::Ref<Seek::PhysicsWorld2D> m_World;
    Seek::Ref<Seek::PhysicsBox2D> m_Box;
    Seek::Ref<Seek::PhysicsBox2D> m_Box2;
    Seek::Ref<Seek::PhysicsBox2D> m_GroundBox;

    Seek::Ref<Seek::Font> m_Font;
    Seek::Ref<Seek::Font> m_Font2;

    Menu* m_Menu = nullptr;

    float m_Scale = 1.0f;

    float m_Progress = 0.0f;
};