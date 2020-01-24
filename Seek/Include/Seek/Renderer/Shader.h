#pragma once

#include "Seek/Core.h"
#include "Seek/Log.h"

#include <glm/glm.hpp>

namespace Seek
{
    class Shader
    {
    public:
        virtual ~Shader() {}

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void SetUniformInt(const String& name, int32 value) = 0;
        virtual void SetUnifromIntArray(const String& name, int32* values,
                                        uint32 count) = 0;
        virtual void SetUniformFloat(const String& name, float32 value) = 0;
        virtual void SetUniformFloat2(const String& name,
                                      const glm::vec2& value) = 0;
        virtual void SetUniformFloat3(const String& name,
                                      const glm::vec3& value) = 0;
        virtual void SetUniformFloat4(const String& name,
                                      const glm::vec4& value) = 0;
        virtual void SetUniformMatrix4(const String& name,
                                       const glm::mat4& matrix) = 0;

        virtual const String& GetName() const = 0;

        static Ref<Shader> Create(const String& filePath);
        static Ref<Shader> Create(const String& vertPath,
                                  const String& fragPath);
        static Ref<Shader> Create(const String& name,
                                  const String& vertexSource,
                                  const String& fragmentSource);
    };

    class ShaderLibrary
    {
    public:
        void Add(const Ref<Shader>& shader);
        void Add(const String& name, const Ref<Shader>& shader);
        Ref<Shader> Load(const String& filePath);
        Ref<Shader> Load(const String& name, const String& filePath);

        Ref<Shader> Get(const String& name);

    private:
        std::unordered_map<String, Ref<Shader>> m_Shaders;
    };
}