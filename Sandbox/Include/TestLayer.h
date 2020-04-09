#pragma once

#include <Seek.h>
#include <imgui.h>
#include <box2d/box2d.h>

class TestLayer : public Seek::Layer
{
public:
    TestLayer();

    void OnAttach() override;
    void OnDetach() override;

    void OnUpdate(Seek::Timestep ts) override;
    void OnImGuiRender(Seek::Timestep ts) override;

private:
    Seek::OrthographicCamera m_Camera;
    Seek::Ref<Seek::Texture2D> m_Texture;

    Seek::Ref<Seek::Sound> m_Sound;

    Seek::Ref<Seek::PhysicsWorld2D> m_World;
    Seek::Ref<Seek::PhysicsBox2D> m_Box;
    Seek::Ref<Seek::PhysicsBox2D> m_Box2;
    Seek::Ref<Seek::PhysicsBox2D> m_GroundBox;

    Seek::Ref<Seek::Font> m_Font;

    float m_Progress = 0.0f;
};