#include "SeekPCH.h"
#include "Seek/UI/Transition/Animator.h"

#include "Seek/UI/UIComponent.h"

namespace Seek
{
    Animator::Animator(UIComponent* component) : m_Component(component)
    {
        ResetValues();
    }

    Animator::~Animator() { m_Component = nullptr; }

    void Animator::ApplyModifier(Transition* transition, bool reverse,
                                 float delay)
    {
        Modifier* oldModifier = nullptr;
        if (m_Modifiers.find(transition) != m_Modifiers.end())
        {
            Modifier* oldModifier = m_Modifiers[transition];
            m_Modifiers.erase(transition);
        }

        Modifier* newModifier =
            transition->CreateModifier(oldModifier, reverse, delay);
        m_Modifiers[transition] = newModifier;
    }

    bool Animator::IsDoingTransition(Transition* transition)
    {
        if (m_Modifiers.find(transition) == m_Modifiers.end())
            return false;

        Modifier* modifier = m_Modifiers[transition];

        return !modifier->HasFinishedTransition();
    }

    void Animator::UpdateComponent()
    {
        if (m_PosChange)
        {
            m_Component->NotifyDimensionChange(m_SizeChange);
        }

        if (m_AlphaChange)
        {
            // m_Component->UpdateTotalAlpha();
        }
    }

    void Animator::Update(float deltaTime)
    {
        if (m_Modifiers.empty())
            return;

        ResetValues();

        for (auto it = m_Modifiers.cbegin(); it != m_Modifiers.cend();)
        {
            (*it).second->Update(deltaTime, this);
            if ((*it).second->IsRedundant())
                m_Modifiers.erase(it++);
            else
                ++it;
        }

        UpdateComponent();
    }

    void Animator::ApplyX(float xValue, bool change)
    {
        m_XValue += xValue;
        m_PosChange |= change;
    }

    void Animator::ApplyY(float yValue, bool change)
    {
        m_YValue += yValue;
        m_PosChange |= change;
    }

    void Animator::ApplyWidth(float width, bool change)
    {
        m_Width *= width;
        m_SizeChange |= change;
        m_PosChange |= change;
    }

    void Animator::ApplyHeight(float height, bool change)
    {
        m_Height *= height;
        m_SizeChange |= change;
        m_PosChange |= change;
    }

    void Animator::ApplyAlpha(float alpha, bool change)
    {
        m_Alpha *= alpha;
        m_AlphaChange |= change;
    }

    void Animator::ResetValues()
    {
        m_PosChange = false;
        m_SizeChange = false;
        m_AlphaChange = false;

        m_XValue = 0;
        m_YValue = 0;
        m_Width = 1;
        m_Height = 1;
        m_Alpha = 1;
    }
}