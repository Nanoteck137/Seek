#include "SeekPCH.h"
#include "Seek/Renderer/TextureManager.h"

namespace Seek
{
    struct TextureManagerStorage
    {
        std::unordered_map<String, Ref<Texture2D>> Textures;
    };

    static TextureManagerStorage* s_Data = nullptr;

    void TextureManager::Init()
    {
        SK_CORE_ASSERT(!s_Data, "TextureManager already initialized");

        s_Data = new TextureManagerStorage();
    }

    void TextureManager::Shutdown()
    {
        SK_CORE_ASSERT(s_Data, "TextureManager already shutdown");

        if (s_Data)
        {
            delete s_Data;
            s_Data = nullptr;
        }
    }

    Ref<Texture2D> TextureManager::LoadTexture(const String& name,
                                               const String& filename,
                                               TextureParameters params)
    {
        SK_CORE_ASSERT(s_Data->Textures.find(name) == s_Data->Textures.end(),
                       "Texture already exists");

        // TODO(patrik): Chack if the texture file exists

        Ref<Texture2D> texture = Texture2D::Create(filename, params);
        s_Data->Textures[name] = texture;

        return texture;
    }

    Ref<Texture2D> TextureManager::GetTexture(const String& name)
    {
        SK_CORE_ASSERT(s_Data->Textures.find(name) != s_Data->Textures.end(),
                       "No texture with name 'insert name'");

        return s_Data->Textures[name];
    }
}