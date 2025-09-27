//
// Created by pointerlost on 9/17/25.
//
#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/ext.hpp>

namespace Zeroday {

    enum CameraMode { Perspective = 0, Orthographic = 1 };

    class Camera {
    public:
        Camera() : Camera(CameraMode::Perspective) {}
        explicit Camera(CameraMode mode) : m_Mode(mode) {}

        void SetPerspectiveFov(float fov) {
            m_PerspectiveFOV = fov;
            m_ProjectionDirty = true;
            m_ViewProjectionDirty = true;
        }
        [[nodiscard]] float GetPerspectiveFOV()   const { return m_PerspectiveFOV;   }
        void SetPerspectiveNear(float near) {
            m_PerspectiveNear = near;
            m_ProjectionDirty = true;
            m_ViewProjectionDirty = true;
        }
        [[nodiscard]] float GetPerspectiveNear()  const { return m_PerspectiveNear;  }
        void SetPerspectiveFar(float far) {
            m_PerspectiveFar = far;
            m_ProjectionDirty = true;
            m_ViewProjectionDirty = true;
        }
        [[nodiscard]] float GetPerspectiveFar()   const { return m_PerspectiveFar;   }
        void SetOrthographicFar(float far) {
            m_OrthographicFar = far;
            m_ProjectionDirty = true;
            m_ViewProjectionDirty = true;
        }
        [[nodiscard]] float GetOrthographicFar()  const { return m_OrthographicFar;  }
        void SetOrthographicSize(float size) {
            m_OrthographicSize = size;
            m_ProjectionDirty = true;
            m_ViewProjectionDirty = true;
        }
        [[nodiscard]] float GetOrthographicSize() const { return m_OrthographicSize; }
        void SetOrthographicNear(float near) {
            m_OrthographicNear = near;
            m_ProjectionDirty = true;
            m_ViewProjectionDirty = true;
        }
        [[nodiscard]] float GetOrthographicNear() const { return m_OrthographicNear; }

        void SetViewportSize(uint32_t width, uint32_t height) {
            m_AspectRatio = (float)width / (float)height;
            m_ProjectionDirty = true;
            m_ViewProjectionDirty = true;
        }
        [[nodiscard]] float GetViewportSize() const { return m_AspectRatio; }

        void UpdateViewMatrix(const glm::mat4& cameraWorldMatrix) {
            m_View = glm::inverse(cameraWorldMatrix);
            m_ViewProjectionDirty = true;
        }
        [[nodiscard]] const glm::mat4& GetViewMatrix() { return m_View; }

        void SetProjectionMatrix(float fov, float near, float far) {
            m_PerspectiveFOV  = fov;
            m_PerspectiveNear = near;
            m_PerspectiveFar  = far;
            m_ProjectionDirty = true;
            m_ViewProjectionDirty = true;
        }
        [[nodiscard]] const glm::mat4& GetProjectionMatrix() {
            CalculateProjection();
            return m_Projection;
        }

        const glm::mat4& GetProjectionViewMatrix() {
            if (m_ViewProjectionDirty || m_ProjectionDirty) {
                CalculateProjection();
                m_ViewProjection = m_Projection * m_View;
                m_ViewProjectionDirty = false;
            }
            return m_ViewProjection;
        }

    private:
        float m_PerspectiveFOV  = glm::radians(45.0f);
        float m_PerspectiveNear = 0.01f, m_PerspectiveFar = 1000.0f;

        float m_OrthographicSize = 10.0f;
        float m_OrthographicNear = -1.0f, m_OrthographicFar = 1.0f;

        float m_AspectRatio = 0.0f;
        CameraMode m_Mode = CameraMode::Perspective;

        glm::mat4 m_View       = glm::mat4(1.0f);
        glm::mat4 m_Projection = glm::mat4(1.0f);
        glm::mat4 m_ViewProjection = glm::mat4(1.0f);

        mutable bool m_ProjectionDirty = true;
        mutable bool m_ViewProjectionDirty = true;

        void CalculateProjection();
    };
}