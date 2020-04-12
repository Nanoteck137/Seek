#include "SeekPCH.h"
#include "Seek/UI/Components/UIContainer.h"

namespace Seek
{
    UIContainer::UIContainer()
    {
        ForceInit(glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f));
    }

    UIContainer::~UIContainer() {}
}