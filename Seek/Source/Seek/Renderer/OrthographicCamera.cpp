#include "SeekPCH.h"
#include "Seek/Renderer/OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Seek/App/Application.h"

namespace Seek
{
    OrthographicCamera::OrthographicCamera(float32 size)
        : m_OrthograpicSize(size)
    {
        RecalculateProjectionMatrix();
        RecalculateViewMatrix();
    }

    OrthographicCamera::~OrthographicCamera() {}

    void OrthographicCamera::Update()
    {
        RecalculateProjectionMatrix();
        RecalculateViewMatrix();
    }

    glm::vec2
    OrthographicCamera::ConvertScreenToWorldPoint(const glm::vec2& screenCoords)
    {
        Seek::Window& window = Seek::Application::Get().GetWindow();
        uint32 width = window.GetWidth();
        uint32 height = window.GetHeight();

        glm::vec2 cameraSize = GetCameraSize();

        glm::vec2 worldPos =
            glm::vec2(screenCoords.x * (cameraSize.x / (float32)width),
                      screenCoords.y * (cameraSize.y / (float32)height));

        worldPos -= glm::vec2(cameraSize.x / 2.0f, cameraSize.y / 2.0f);
        worldPos += m_Position;

        return worldPos;
    }

    glm::vec2 OrthographicCamera::GetCameraSize()
    {
        Seek::Window& window = Seek::Application::Get().GetWindow();
        uint32 width = window.GetWidth();
        uint32 height = window.GetHeight();

        float32 aspect = (float32)width / (float32)height;

        float32 camHeight = 2.0f * m_OrthograpicSize;
        float32 camWidth = camHeight * aspect;

        return {camWidth, camHeight};
    }

    void OrthographicCamera::RecalculateProjectionMatrix()
    {
        Window& window = Application::Get().GetWindow();
        float32 aspect =
            (float32)window.GetWidth() / (float32)window.GetHeight();

        m_ProjectionMatrix =
            glm::ortho(-m_OrthograpicSize * aspect, m_OrthograpicSize * aspect,
                       -m_OrthograpicSize, m_OrthograpicSize);

        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

    void OrthographicCamera::RecalculateViewMatrix()
    {
        glm::vec3 position = glm::vec3(m_Position.x, m_Position.y, 0.0f);
        glm::mat4 transform =
            glm::translate(glm::mat4(1.0f), position) *
            glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation),
                        glm::vec3(0.0f, 0.0f, 1.0f));

        m_ViewMatrix = glm::inverse(transform);
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }
}
