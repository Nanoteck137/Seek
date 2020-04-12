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

    void UIComponent::Add(UIComponent* child, UIConstraints* constraints)
    {
        child->m_Parent = this;
        child->m_Constraints = constraints;
        constraints->NotifyAdded(child, this);

        InitChild(child);
    }

    void UIComponent::AddInStyle(UIComponent* child, UIConstraints* constraints)
    {
        child->SetHidden();
        Add(child, constraints);
        child->Display(true);
    }

    void UIComponent::SetHidden()
    {
        m_Displayed = false;
        SetVisable(false);
    }

    void UIComponent::Display(bool display)
    {
        if (display == m_Displayed)
            return;

        m_Displayed = display;

        if (display)
            SetVisable(true);

        DoDisplayAnimation(display, 0.0f, true);
    }

    void UIComponent::SetDisplayTransition(const Ref<Transition>& transition)
    {
        m_DisplayTransition = transition;
        m_DisplayDelay = 0.0f;
        m_HideDelay = 0.0f;
    }

    void UIComponent::SetDisplayTransition(const Ref<Transition>& transition,
                                           float32 displayDelay,
                                           float32 hideDelay)
    {
        m_DisplayTransition = transition;
        m_DisplayDelay = displayDelay;
        m_HideDelay = hideDelay;
    }

    void UIComponent::Update(Timestep ts)
    {
        if (!m_Visable)
            return;

        m_Animator->Update(ts);

        OnUpdate(ts);
        for (int i = 0; i < m_Children.size(); i++)
        {
            m_Children[i]->Update(ts);
        }
    }

    void UIComponent::NotifyDimensionChange(bool sizeChange)
    {
        CalculateScreenSpacePosition(sizeChange);
        OnDimentionsChange();

        for (UIComponent* child : m_Children)
        {
            child->NotifyDimensionChange(sizeChange);
        }
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

    glm::vec2 UIComponent::GetAnimationSize() { return m_Animator->GetSize(); }

    glm::vec2 UIComponent::GetAnimationPosition()
    {
        glm::vec2 position = m_Animator->GetPosition();
        position.x *= m_Constraints->GetWidth()->GetRelativeValue();
        position.y *= m_Constraints->GetHeight()->GetRelativeValue();
        return position;
    }

    float32 UIComponent::GetAbsAspectRatio()
    {
        glm::vec2 size = GetSizeInPixels();
        return size.x / size.y;
    }

    float32 UIComponent::GetRelativeWidthCoords(float32 relativeWidth)
    {
        return relativeWidth / GetAbsAspectRatio();
    }

    float32 UIComponent::GetRelativeHeightCoords(float32 relativeHeight)
    {
        return relativeHeight * GetAbsAspectRatio();
    }

    void UIComponent::ForceInit(const glm::vec2& position,
                                const glm::vec2& size)
    {
        m_Position = position;
        m_Size = size;
    }

    void UIComponent::UpdateTotalAlpha()
    {
        CalcTotalAlpha();

        for (auto& it : m_Children)
            it->UpdateTotalAlpha();
    }

    void UIComponent::CalcTotalAlpha()
    {
        float animationAlpha = m_Animator->GetAlpha();
        m_TotalAlpha = m_Parent->m_TotalAlpha * m_Alpha * animationAlpha;
    }

    void UIComponent::CalculateScreenSpacePosition(bool calcSize)
    {
        glm::vec2 parentPosition = m_Parent->GetPosition();
        glm::vec2 parentSize = m_Parent->GetSize();

        glm::vec2 animationPosition = GetAnimationPosition();
        glm::vec2 animationSize = GetAnimationSize();

        m_Position.x =
            parentPosition.x +
            (m_Constraints->GetX()->GetRelativeValue() + animationPosition.x) *
                parentSize.x;
        m_Position.y =
            parentPosition.y +
            (m_Constraints->GetY()->GetRelativeValue() + animationPosition.y) *
                parentSize.y;

        if (calcSize)
        {
            m_Size.x = m_Constraints->GetWidth()->GetRelativeValue() *
                       parentSize.x * animationSize.x;
            m_Size.y = m_Constraints->GetHeight()->GetRelativeValue() *
                       parentSize.y * animationSize.y;
        }
    }

    void UIComponent::InitChild(UIComponent* child)
    {
        m_Children.push_back(child);
        child->CalculateScreenSpacePosition(true);
        child->OnInit();
    }

    void UIComponent::DoDisplayAnimation(bool display, float32 parentDelay,
                                         bool head)
    {
        if (!IsVisable() || (!m_Displayed && !head))
            return;

        float32 delay = display ? m_DisplayDelay : m_HideDelay;
        if (m_DisplayTransition)
        {
            m_Animator->ApplyModifier(m_DisplayTransition, display, delay);
        }

        for (auto& it : m_Children)
            it->DoDisplayAnimation(display, delay, false);
    }
}