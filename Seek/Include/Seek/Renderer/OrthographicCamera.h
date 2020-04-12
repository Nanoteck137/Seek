#pragma once

#include "Seek/Core.h"
#include "Seek/Log.h"

#include <glm/glm.hpp>

namespace Seek
{
    class OrthographicCamera
    {
    public:
        OrthographicCamera(float left, float right, float bottom, float top);
        ~OrthographicCamera();

        inline const glm::mat4& GetProjectionMatrix() const
        {
            return m_ProjectionMatrix;
        }

        inline const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }

        inline const glm::mat4& GetViewProjectionMatrix() const
        {
            return m_ViewProjectionMatrix;
        }

        inline const glm::vec3& GetPosition() const { return m_Position; }
        inline void SetPosition(const glm::vec3& position)
        {
            m_Position = position;
            RecalculateViewMatrix();
        }

        inline float GetRotation() const { return m_Rotation; }
        inline void SetRotation(float rotation)
        {
            m_Rotation = rotation;
            RecalculateViewMatrix();
        }

    private:
        void RecalculateViewMatrix();

    private:
        glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
        glm::mat4 m_ViewMatrix = glm::mat4(1.0f);
        glm::mat4 m_ViewProjectionMatrix = glm::mat4(1.0f);

        glm::vec3 m_Position = {0.0f, 0.0f, 0.0f};
        float m_Rotation = 0.0f;
    };
}