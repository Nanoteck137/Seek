#pragma once

#include "Seek/Core.h"
#include "Seek/Log.h"

#include <glm/glm.hpp>

namespace Seek
{
    class OrthographicCamera
    {
    public:
        OrthographicCamera(float32 size = 5.0f);

        ~OrthographicCamera();

        void Update();

        glm::vec2 ConvertScreenToWorldPoint(const glm::vec2& screenCoords);

        inline const glm::mat4& GetProjectionMatrix() const
        {
            return m_ProjectionMatrix;
        }

        inline float32 GetOrthograpicSize() const { return m_OrthograpicSize; }
        inline void SetOrthograpicSize(float32 size)
        {
            m_OrthograpicSize = size;
            RecalculateProjectionMatrix();
        }

        inline const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }

        inline const glm::mat4& GetViewProjectionMatrix() const
        {
            return m_ViewProjectionMatrix;
        }

        inline void SetPosition(const glm::vec2& position)
        {
            m_Position = position;
            RecalculateViewMatrix();
        }
        inline const glm::vec2& GetPosition() const { return m_Position; }

        inline void SetRotation(float32 rotation)
        {
            m_Rotation = rotation;
            RecalculateViewMatrix();
        }
        inline float32 GetRotation() const { return m_Rotation; }

    private:
        void RecalculateProjectionMatrix();
        void RecalculateViewMatrix();

    private:
        float32 m_OrthograpicSize = 0.0f;

        glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
        glm::mat4 m_ViewMatrix = glm::mat4(1.0f);
        glm::mat4 m_ViewProjectionMatrix = glm::mat4(1.0f);

        glm::vec2 m_Position = glm::vec2(0.0f, 0.0f);
        float32 m_Rotation = 0.0f;
    };
}