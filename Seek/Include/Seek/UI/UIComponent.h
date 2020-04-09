#pragma once

#include "Seek/Core.h"
#include "Seek/Log.h"

#include <glm/glm.hpp>

#include "Seek/UI/Transition/Animator.h"

#include "Seek/Events/Event.h"
#include "Seek/Events/MouseEvent.h"

namespace Seek
{
    class UIConstraints;

    class UIComponent
    {
    protected:
        UIComponent();

    public:
        virtual ~UIComponent();

        void Add(UIComponent* child, UIConstraints* constraints);

        void Update(float deltaTime);

        float GetPixelWidth();
        float GetPixelHeight();

        float GetAnimationWidth();
        float GetAnimationHeight();
        float GetAnimationX();
        float GetAnimationY();

        glm::vec2 GetPositionInPixels();
        glm::vec2 GetSizeInPixels();

        void NotifyDimensionChange(bool sizeChange);

    public:
        virtual void OnEvent(Event& event);

    protected:
        void ForceInit(float absX, float absY, float absWidth, float absHeight);

        void CalculateScreenSpacePosition(bool calcSize);
        void InitChild(UIComponent* child);

    protected:
        virtual void Init() {}
        virtual void UpdateSelf(float deltaTime) = 0;

    public:
        inline const std::vector<UIComponent*>& GetChildren() const
        {
            return m_Children;
        }

        inline Animator* GetAnimator() { return m_Animator; }

    protected:
        std::vector<UIComponent*> m_Children;
        UIComponent* m_Parent = nullptr;
        UIConstraints* m_Constraints = nullptr;

        Animator* m_Animator = nullptr;

        glm::vec2 m_Position;
        glm::vec2 m_Size;
    };
}