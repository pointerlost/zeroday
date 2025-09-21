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

        void SetPerspectiveFov(float fov)     { m_PerspectiveFOV = fov; CalculateProjection();    }
        [[nodiscard]] float GetPerspectiveFOV()   const { return m_PerspectiveFOV;   }
        void SetPerspectiveNear(float near)   { m_PerspectiveNear = near; CalculateProjection();  }
        [[nodiscard]] float GetPerspectiveNear()  const { return m_PerspectiveNear;  }
        void SetPerspectiveFar(float far)     { m_PerspectiveFar = far; CalculateProjection();    }
        [[nodiscard]] float GetPerspectiveFar()   const { return m_PerspectiveFar;   }
        void SetOrthographicFar(float far)    { m_OrthographicFar = far; CalculateProjection();   }
        [[nodiscard]] float GetOrthographicFar()  const { return m_OrthographicFar;  }
        void SetOrthographicSize(float size)  { m_OrthographicSize = size; CalculateProjection(); }
        [[nodiscard]] float GetOrthographicSize() const { return m_OrthographicSize; }
        void SetOrthographicNear(float near)  { m_OrthographicNear = near; CalculateProjection(); }
        [[nodiscard]] float GetOrthographicNear() const { return m_OrthographicNear; }

        void SetViewportSize(uint32_t width, uint32_t height) { m_AspectRatio = static_cast<float>(width) / static_cast<float>(height); CalculateProjection(); }
        [[nodiscard]] float GetViewportSize() const { return m_AspectRatio; }

        [[nodiscard]] const glm::mat4& GetProjectionMatrix() const;
        [[nodiscard]] const glm::mat4& GetViewMatrix() const;

    private:
        float m_PerspectiveFOV  = glm::radians(45.0f);
        float m_PerspectiveNear = 0.01f, m_PerspectiveFar = 1000.0f;

        float m_OrthographicSize = 10.0f;
        float m_OrthographicNear = -1.0f, m_OrthographicFar = 1.0f;

        float m_AspectRatio = 0.0f;
        CameraMode m_Mode = CameraMode::Perspective;

        glm::mat4 m_View       = glm::mat4(1.0f);
        glm::mat4 m_Projection = glm::mat4(1.0f);

        void CalculateProjection();
    };
}
