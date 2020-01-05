#pragma once

#include "Seek/Core.h"
#include "Seek/Log.h"

namespace Seek
{
    class Shader
    {
    public:
        virtual ~Shader() {}

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        static Ref<Shader> Create(const String& vertexSource,
                                  const String& fragmentShader);
    };
}