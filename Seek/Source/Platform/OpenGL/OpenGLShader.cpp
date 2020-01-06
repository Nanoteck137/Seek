#include "SeekPCH.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

#include <fstream>

namespace Seek
{
    static uint32 ShaderTypeFromString(const String& type)
    {
        if (type == "vertex")
            return GL_VERTEX_SHADER;
        if (type == "fragment" || type == "pixel")
            return GL_FRAGMENT_SHADER;

        SK_CORE_ASSERT(false, "Unknown shader type!");
        return 0;
    }

    OpenGLShader::OpenGLShader(const String& filePath)
    {
        String shaderSource = ReadFile(filePath);
        auto shaderSources = PreProcess(shaderSource);
        Compile(shaderSources);

        auto lastSlash = filePath.find_last_of("/\\");
        lastSlash = lastSlash == String::npos ? 0 : lastSlash + 1;

        auto lastDot = filePath.rfind('.');
        auto count = lastDot == String::npos ? filePath.size() - lastSlash
                                             : lastDot - lastSlash;
        m_Name = filePath.substr(lastSlash, count);
    }

    OpenGLShader::OpenGLShader(const String& name, const String& vertexSource,
                               const String& fragmentSource)
        : m_Name(name)
    {
        std::unordered_map<uint32, String> sources;
        sources[GL_VERTEX_SHADER] = vertexSource;
        sources[GL_FRAGMENT_SHADER] = fragmentSource;
        Compile(sources);
    }

    OpenGLShader::~OpenGLShader() { glDeleteProgram(m_RendererID); }

    String OpenGLShader::ReadFile(const String& filePath)
    {
        std::string result;
        std::ifstream in(filePath, std::ios::in, std::ios::binary);
        if (in)
        {
            in.seekg(0, std::ios::end);
            result.resize(in.tellg());
            in.seekg(0, std::ios::beg);

            in.read(&result[0], result.size());
            in.close();
        }
        else
        {
            SK_CORE_ERROR("Could not open file '{0}'", filePath);
        }

        return result;
    }

    std::unordered_map<uint32, String>
    OpenGLShader::PreProcess(const String& source)
    {
        std::unordered_map<uint32, String> shaderSources;

        const char* typeToken = "#type";
        size_t typeTokenLength = strlen(typeToken);
        size_t pos = source.find(typeToken, 0);
        while (pos != String::npos)
        {
            size_t eol = source.find_first_of("\r\n", pos);
            SK_CORE_ASSERT(eol != String::npos, "Syntax error");

            size_t begin = pos + typeTokenLength + 1;
            String type = source.substr(begin, eol - begin);
            SK_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type");

            size_t nextLinePos = source.find_first_not_of("\r\n", eol);
            pos = source.find(typeToken, nextLinePos);
            shaderSources[ShaderTypeFromString(type)] =
                source.substr(nextLinePos, pos - (nextLinePos == String::npos
                                                      ? source.size() - 1
                                                      : nextLinePos));
        }

        return shaderSources;
    }

    void OpenGLShader::Compile(
        const std::unordered_map<uint32, String>& shaderSources)
    {
        uint32 program = glCreateProgram();

        std::vector<GLuint> glShaderIDs;
        glShaderIDs.reserve(shaderSources.size());

        for (auto& kv : shaderSources)
        {
            GLenum type = kv.first;
            const String& source = kv.second;

            GLuint shader = glCreateShader(type);
            const GLchar* sourceCStr = (const GLchar*)source.c_str();
            glShaderSource(shader, 1, &sourceCStr, 0);

            glCompileShader(shader);

            GLint isCompiled = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
            if (isCompiled == GL_FALSE)
            {
                GLint maxLength = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

                std::vector<GLchar> infoLog(maxLength);
                glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

                glDeleteShader(shader);

                SK_CORE_ERROR("{0}", infoLog.data());
                SK_CORE_ASSERT(false, "Shader compilation failed");

                break;
            }

            glAttachShader(program, shader);
            glShaderIDs.push_back(shader);
        }

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

            for (auto id : glShaderIDs)
                glDeleteShader(id);

            SK_CORE_ERROR("{0}", infoLog.data());
            SK_CORE_ASSERT(false, "Failed to link shaders");

            return;
        }

        for (auto id : glShaderIDs)
            glDetachShader(program, id);

        m_RendererID = program;
    }

    void OpenGLShader::Bind() const { glUseProgram(m_RendererID); }

    void OpenGLShader::Unbind() const { glUseProgram(0); }

    void OpenGLShader::SetUniformInt(const String& name, int32 value)
    {
        int32 loc = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform1i(loc, value);
    }

    void OpenGLShader::SetUnifromIntArray(const String& name, int32* values,
                                          uint32 count)
    {
        // TODO(patrik): Check loc after -1
        int32 loc = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform1iv(loc, count, values);
    }

    void OpenGLShader::SetUniformFloat(const String& name, float32 value)
    {
        int32 loc = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform1f(loc, value);
    }

    void OpenGLShader::SetUniformFloat2(const String& name,
                                        const glm::vec2& value)
    {
        int32 loc = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform2f(loc, value.x, value.y);
    }

    void OpenGLShader::SetUniformFloat3(const String& name,
                                        const glm::vec3& value)
    {
        int32 loc = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform3f(loc, value.x, value.y, value.z);
    }

    void OpenGLShader::SetUniformFloat4(const String& name,
                                        const glm::vec4& value)
    {
        int32 loc = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform4f(loc, value.x, value.y, value.z, value.w);
    }

    void OpenGLShader::SetUniformMatrix4(const String& name,
                                         const glm::mat4& matrix)
    {
        int32 loc = glGetUniformLocation(m_RendererID, name.c_str());
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
    }
}