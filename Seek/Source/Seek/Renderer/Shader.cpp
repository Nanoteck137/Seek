#include "SeekPCH.h"
#include "Seek/Renderer/Shader.h"

#include "Seek/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Seek
{
    Ref<Shader> Shader::Create(const String& filePath)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:
                SK_CORE_ASSERT(false, "RendererAPI::None is not supported!")
                return nullptr;
            case RendererAPI::API::OpenGL:
                return CreateRef<OpenGLShader>(filePath);
        }

        SK_CORE_ASSERT(false, "No RenderingAPI selected");
        return nullptr;
    }

    Ref<Shader> Shader::Create(const String& name, const String& vertexSource,
                               const String& fragmentSource)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:
                SK_CORE_ASSERT(false, "RendererAPI::None is not supported!")
                return nullptr;
            case RendererAPI::API::OpenGL:
                return CreateRef<OpenGLShader>(name, vertexSource,
                                               fragmentSource);
        }

        SK_CORE_ASSERT(false, "No RenderingAPI selected");
        return nullptr;
    }

    void ShaderLibrary::Add(const Ref<Shader>& shader)
    {
        auto& name = shader->GetName();
        SK_CORE_ASSERT(m_Shaders.find(name) == m_Shaders.end(),
                       "Shader already exits");
        m_Shaders[name] = shader;
    }

    void ShaderLibrary::Add(const String& name, const Ref<Shader>& shader)
    {
        SK_CORE_ASSERT(m_Shaders.find(name) == m_Shaders.end(),
                       "Shader already exits");
        m_Shaders[name] = shader;
    }

    Ref<Shader> ShaderLibrary::Load(const String& filePath)
    {
        auto shader = Shader::Create(filePath);
        Add(shader);
        return shader;
    }

    Ref<Shader> ShaderLibrary::Load(const String& name, const String& filePath)
    {
        auto shader = Shader::Create(filePath);
        Add(name, shader);
        return shader;
    }

    Ref<Shader> ShaderLibrary::Get(const String& name)
    {
        SK_CORE_ASSERT(m_Shaders.find(name) != m_Shaders.end(),
                       "Shader not found");
        return Ref<Shader>();
    }

}