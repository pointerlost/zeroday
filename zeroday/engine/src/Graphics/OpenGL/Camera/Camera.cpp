//
// Created by pointerlost on 9/17/25.
//
#include "Graphics/OpenGL/Camera/Camera.h"

namespace Zeroday {

    void Camera::CalculateProjection() {
        if (!m_ProjectionDirty) return;

        m_ProjectionDirty = false;

        if (m_Mode == CameraMode::Perspective) {
            m_Projection = glm::perspective(m_PerspectiveFOV, m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
            return;
        }

        const float halfWidth  = m_OrthographicSize * m_AspectRatio * 0.5f;
        const float halfHeight = m_OrthographicSize * 0.5f;
        m_Projection = glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, m_OrthographicNear, m_OrthographicFar);
    }
}
