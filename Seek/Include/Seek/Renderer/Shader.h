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
        virtual void SetUniformMatrix4(const String& name,
                                       const glm::mat4& matrix) = 0;

        static Ref<Shader> Create(const String& vertexSource,
                                  const String& fragmentShader);
    };
}