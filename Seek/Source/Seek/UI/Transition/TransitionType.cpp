#include "SeekPCH.h"
#include "Seek/UI/Transition/TransitionType.h"

#include "Seek/UI/Transition/Animator.h"

namespace Seek
{
    TransitionType* TransitionType::XPOS = new TransitionType(
        0.0f, [=](Animator* animator, float value, bool change) {
            animator->ApplyX(value, change);
        });
    TransitionType* TransitionType::YPOS = new TransitionType(
        0.0f, [=](Animator* animator, float value, bool change) {
            animator->ApplyY(value, change);
        });
    TransitionType* TransitionType::WIDTH = new TransitionType(
        1.0f, [=](Animator* animator, float value, bool change) {
            animator->ApplyWidth(value, change);
        });
    TransitionType* TransitionType::HEIGHT = new TransitionType(
        1.0f, [=](Animator* animator, float value, bool change) {
            animator->ApplyHeight(value, change);
        });
    TransitionType* TransitionType::ALPHA = new TransitionType(
        1.0f, [=](Animator* animator, float value, bool change) {
            animator->ApplyAlpha(value, change);
        });

    TransitionType::TransitionType(
        float baseValue, std::function<void(Animator*, float, bool)> setter)
        : m_BaseValue(baseValue), m_Setter(setter)
    {
    }

    void TransitionType::ApplyValue(Animator* animator, float value,
                                    bool change)
    {
        m_Setter(animator, value, change);
    }
}