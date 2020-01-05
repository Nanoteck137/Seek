#include "SeekPCH.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include <glad/glad.h>

namespace Seek
{
    OpenGLShader::OpenGLShader(const String& vertexSource,
                               const String& fragmentSource)
    {
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

        const GLchar* source = (const GLchar*)vertexSource.c_str();
        glShaderSource(vertexShader, 1, &source, 0);

        glCompileShader(vertexShader);

        GLint isCompiled = 0;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(vertexShader, maxLength, &maxLength,
                               &infoLog[0]);

            glDeleteShader(vertexShader);

            SK_CORE_ERROR("{0}", infoLog.data());
            SK_CORE_ASSERT(false, "Vertex Shader compilation failed");

            return;
        }

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

        source = (const GLchar*)fragmentSource.c_str();
        glShaderSource(fragmentShader, 1, &source, 0);

        glCompileShader(fragmentShader);

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(fragmentShader, maxLength, &maxLength,
                               &infoLog[0]);

            glDeleteShader(fragmentShader);
            glDeleteShader(vertexShader);

            SK_CORE_ERROR("{0}", infoLog.data());
            SK_CORE_ASSERT(false, "Fragment Shader compilation failed");

            return;
        }

        uint32 program = glCreateProgram();

        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);

        glLinkProgram(program);

        GLint isLinked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

            glDeleteProgram(program);

            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);

            SK_CORE_ERROR("{0}", infoLog.data());
            SK_CORE_ASSERT(false, "Failed to link shaders");

            return;
        }

        glDetachShader(program, vertexShader);
        glDetachShader(program, fragmentShader);

        m_RendererID = program;
    }

    OpenGLShader::~OpenGLShader() { glDeleteProgram(m_RendererID); }

    void OpenGLShader::Bind() const { glUseProgram(m_RendererID); }

    void OpenGLShader::Unbind() const { glUseProgram(0); }
}