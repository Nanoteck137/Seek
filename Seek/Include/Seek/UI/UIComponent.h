#pragma once

#include "Seek/Core.h"
#include "Seek/Log.h"

#include <glm/glm.hpp>

#include "Seek/App/Timestep.h"

#include "Seek/UI/Transition/Animator.h"

#include "Seek/Events/Event.h"
#include "Seek/Events/MouseEvent.h"

namespace Seek
{
    class UIConstraints;

    class UIComponent
    {
    public:
        friend class Animator;

    protected:
        UIComponent();

    public:
        virtual ~UIComponent();

        virtual void OnEvent(Event& event);

        void Add(UIComponent* child, UIConstraints* constraints);
        void AddInStyle(UIComponent* child, UIConstraints* constraints);

        void SetHidden();
        void Display(bool display);

        void SetDisplayTransition(const Ref<Transition>& transition);

        void SetDisplayTransition(const Ref<Transition>& transition,
                                  float32 displayDelay, float32 hideDelay);

        void Update(Timestep ts);

        void NotifyDimensionChange(bool sizeChange);

        glm::vec2 GetPositionInPixels();
        glm::vec2 GetSizeInPixels();

        glm::vec2 GetAnimationSize();
        glm::vec2 GetAnimationPosition();

        float32 GetAbsAspectRatio();
        float32 GetRelativeWidthCoords(float32 relativeWidth);
        float32 GetRelativeHeightCoords(float32 relativeHeight);

        inline void SetVisable(bool visable) { m_Visable = visable; }
        inline bool IsVisable() const { return m_Visable; }

        inline bool IsDisplayed() const { return m_Displayed; }

        inline Animator& GetAnimator() { return *m_Animator; }
        inline UIConstraints* GetConstraints() const { return m_Constraints; }

        inline const glm::vec2& GetPosition() const { return m_Position; }
        inline const glm::vec2& GetSize() const { return m_Size; }

        inline const std::vector<UIComponent*>& GetChildren() const
        {
            return m_Children;
        }

    protected:
        virtual void OnInit() {}
        virtual void OnUpdate(Timestep ts) {}
        virtual void OnDimentionsChange() {}

        void ForceInit(const glm::vec2& position, const glm::vec2& size);

        void UpdateTotalAlpha();
        void CalcTotalAlpha();
        void CalculateScreenSpacePosition(bool calcSize);

    private:
        void InitChild(UIComponent* child);
        void DoDisplayAnimation(bool display, float32 parentDelay, bool head);

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

        float32 m_TotalAlpha = 1.0f;
        float32 m_Alpha = 1.0f;
    };
}