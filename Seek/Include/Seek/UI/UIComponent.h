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
        void AddInStyle(UIComponent* child, UIConstraints* constraints);

        void SetHidden();
        void Display(bool display);
        void DoDisplayAnimation(bool display, float parentDelay, bool head);

        void Update(float deltaTime);

        float GetPixelWidth();
        float GetPixelHeight();

        float GetAbsAspectRatio();
        float GetRelativeHeightCoords(float relativeWidth);
        float GetRelativeWidthCoords(float relativeHeight);

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
        virtual void OnInit() {}
        virtual void OnUpdate(float deltaTime) {}
        virtual void OnDimentionsChange() {}

    public:
        inline void Show(bool show) { m_Visable = show; }
        inline bool IsShown() { return m_Visable; }

        inline bool IsDisplayed() const { return m_Displayed; }

        inline const std::vector<UIComponent*>& GetChildren() const
        {
            return m_Children;
        }

        inline Animator* GetAnimator() { return m_Animator; }
        inline void SetDisplayTransition(const Ref<Transition>& transition)
        {
            m_DisplayTransition = transition;
            m_DisplayDelay = 0.0f;
            m_HideDelay = 0.0f;
        }

        inline void SetDisplayTransition(const Ref<Transition>& transition,
                                         float32 displayDelay,
                                         float32 hideDelay)
        {
            m_DisplayTransition = transition;
            m_DisplayDelay = displayDelay;
            m_HideDelay = hideDelay;
        }

    protected:
        bool m_Visable = true;
        bool m_Displayed = true;

        std::vector<UIComponent*> m_Children;
        UIComponent* m_Parent = nullptr;
        UIConstraints* m_Constraints = nullptr;

        Animator* m_Animator = nullptr;
        Ref<Transition> m_DisplayTransition = nullptr;
        float32 m_DisplayDelay = 0.0f;
        float32 m_HideDelay = 0.0f;

        glm::vec2 m_Position;
        glm::vec2 m_Size;
    };
}