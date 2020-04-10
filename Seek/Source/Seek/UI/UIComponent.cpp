#include "SeekPCH.h"
#include "Seek/UI/UIComponent.h"

#include "Seek/UI/UIConstraints.h"
#include "Seek/UI/UIManager.h"

namespace Seek
{
    UIComponent::UIComponent() { m_Animator = new Animator(this); }

    UIComponent::~UIComponent()
    {
        for (UIComponent* child : m_Children)
        {
            delete child;
        }

        if (m_Constraints)
        {
            delete m_Constraints;
            m_Constraints = nullptr;
        }

        if (m_Animator)
        {
            delete m_Animator;
            m_Animator = nullptr;
        }
    }

    void UIComponent::Add(UIComponent* child, UIConstraints* constraints)
    {
        child->m_Parent = this;
        child->m_Constraints = constraints;
        constraints->NotifyAdded(child, this);

        InitChild(child);
    }

    void UIComponent::Update(float deltaTime)
    {
        m_Animator->Update(deltaTime);

        UpdateSelf(deltaTime);
        for (int i = 0; i < m_Children.size(); i++)
        {
            m_Children[i]->Update(deltaTime);
        }
    }

    float UIComponent::GetPixelWidth()
    {
        return m_Size.x * UIManager::GetDisplayWidth();
    }

    float UIComponent::GetPixelHeight()
    {
        return m_Size.y * UIManager::GetDisplayHeight();
    }

    float UIComponent::GetAnimationWidth() { return m_Animator->GetWidth(); }

    float UIComponent::GetAnimationHeight() { return m_Animator->GetHeight(); }

    float UIComponent::GetAnimationX()
    {
        return m_Animator->GetX() *
               m_Constraints->GetWidth()->GetRelativeValue();
    }

    float UIComponent::GetAnimationY()
    {
        return m_Animator->GetY() *
               m_Constraints->GetHeight()->GetRelativeValue();
    }

    glm::vec2 UIComponent::GetPositionInPixels()
    {
        return glm::vec2(m_Position.x * UIManager::GetDisplayWidth(),
                         m_Position.y * UIManager::GetDisplayHeight());
    }

    glm::vec2 UIComponent::GetSizeInPixels()
    {
        return glm::vec2(m_Size.x * UIManager::GetDisplayWidth(),
                         m_Size.y * UIManager::GetDisplayHeight());
    }

    void UIComponent::OnEvent(Event& event)
    {
        for (auto it = m_Children.end(); it != m_Children.begin();)
        {
            UIComponent* child = *--it;
            child->OnEvent(event);

            if (event.Handled)
                break;
        }
    }

    void UIComponent::NotifyDimensionChange(bool sizeChange)
    {
        CalculateScreenSpacePosition(sizeChange);

        for (UIComponent* child : m_Children)
        {
            child->NotifyDimensionChange(sizeChange);
        }
    }

    void UIComponent::ForceInit(float absX, float absY, float absWidth,
                                float absHeight)
    {
        m_Position.x = absX;
        m_Position.y = absY;

        m_Size.x = absWidth;
        m_Size.y = absHeight;
    }

    void UIComponent::CalculateScreenSpacePosition(bool calcSize)
    {
        m_Position.x =
            m_Parent->m_Position.x +
            (m_Constraints->GetX()->GetRelativeValue() + GetAnimationX()) *
                m_Parent->m_Size.x;
        m_Position.y =
            m_Parent->m_Position.y +
            (m_Constraints->GetY()->GetRelativeValue() + GetAnimationY()) *
                m_Parent->m_Size.y;

        if (calcSize)
        {
            m_Size.x = m_Constraints->GetWidth()->GetRelativeValue() *
                       m_Parent->m_Size.x * GetAnimationWidth();
            m_Size.y = m_Constraints->GetHeight()->GetRelativeValue() *
                       m_Parent->m_Size.y * GetAnimationHeight();
        }
    }

    void UIComponent::InitChild(UIComponent* child)
    {
        m_Children.push_back(child);
        child->CalculateScreenSpacePosition(true);
        child->Init();
    }
}