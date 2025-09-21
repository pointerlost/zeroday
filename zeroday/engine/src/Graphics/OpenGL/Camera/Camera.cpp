//
// Created by pointerlost on 9/17/25.
//
#include "Graphics/OpenGL/Camera/Camera.h"

namespace Zeroday {

    const glm::mat4& Camera::GetProjectionMatrix() const {
        return m_Projection;
    }

    const glm::mat4& Camera::GetViewMatrix() const {
        return m_View;
    }

    void Camera::CalculateProjection() {
        if (m_Mode == CameraMode::Perspective) {
            m_Projection = glm::perspective(glm::radians(m_PerspectiveFOV), m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
            return;
        }

        // Orthographic
        const float halfWidth  = m_OrthographicSize * m_AspectRatio * 0.5f;
        const float halfHeight = m_OrthographicSize * 0.5f;
        m_Projection = glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, m_OrthographicNear, m_OrthographicFar);
    }
}
