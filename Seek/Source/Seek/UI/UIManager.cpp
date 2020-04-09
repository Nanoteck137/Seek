#include "SeekPCH.h"
#include "Seek/UI/UIManager.h"

namespace Seek
{
    struct UIManagerStorage
    {
        int32 DisplayWidth = 0;
        int32 DisplayHeight = 0;

        UIContainer* Container = nullptr;
    };

    static UIManagerStorage* s_Data;

    void UIManager::Init()
    {
        SK_CORE_ASSERT(!s_Data, "UIManager already initialized");

        s_Data = new UIManagerStorage;
        s_Data->Container = new UIContainer();
    }

    void UIManager::Shutdown()
    {
        SK_CORE_ASSERT(s_Data, "FontManager already shutdown");

        if (s_Data->Container)
        {
            delete s_Data->Container;
            s_Data->Container = nullptr;
        }

        if (s_Data)
        {
            delete s_Data;
            s_Data = nullptr;
        }
    }

    void UIManager::UpdateDisplaySize(int32 width, int32 height)
    {
        s_Data->DisplayWidth = width;
        s_Data->DisplayHeight = height;

        for (UIComponent* component : s_Data->Container->GetChildren())
        {
            component->NotifyDimensionChange(true);
        }
    }

    int32 UIManager::GetDisplayWidth() { return s_Data->DisplayWidth; }

    int32 UIManager::GetDisplayHeight() { return s_Data->DisplayHeight; }

    UIContainer* UIManager::GetContainer() { return s_Data->Container; }
}