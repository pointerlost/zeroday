#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>


namespace Zeroday {

    class Transform {
    public:
        Transform(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale)
            : m_Position(position), m_Rotation(rotation), m_Scale(scale) {}
        Transform() = default;
        Transform(const Transform&) = default;

        // Get Euler angles (degrees) - user friendly (for example UI)
        const glm::vec3& GetEulerRotation() const;

        // set Euler angles (degrees) - converts to quaternion internally
        void SetEulerRotation(const glm::vec3& eulerDegrees);
        // set individual Euler components
        void SetEulerRotation(float x, float y, float z) { SetEulerRotation(glm::vec3(x, y, z)); }

        // --- Quaternion Interface (for internal use) ---

        // get raw quaternion
        const glm::quat& GetRotation() const { return m_Rotation; }

        // set quaternion directly
        void SetRotation(const glm::quat& newRotation) {
            m_Rotation = glm::normalize(newRotation);
            m_EulerDirty = true;
        }

        // --- Rotation Operations ---

        // rotate around axis (quaternion operation)
        void Rotate(float angleDegrees, const glm::vec3& axis);

        // rotate using Euler angles (convenience method)
        void RotateEuler(const glm::vec3& eulerDegrees);

        // look at target point
        void LookAt(const glm::vec3& target, const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f));

        // --- Direction Vectors ---

        glm::vec3 GetForward() const { return m_Rotation * glm::vec3(0.0f, 0.0f, -1.0f); }
        glm::vec3 GetRight()   const { return m_Rotation * glm::vec3(1.0f, 0.0f, 0.0f);  }
        glm::vec3 GetUp()      const { return m_Rotation * glm::vec3(0.0f, 1.0f, 0.0f);  }

        // --- Transformation Matrices ---

        void ReCalculateViewMatrix()  const;
        void ReCalculateModelMatrix() const;
        const glm::mat4& GetViewMatrix()  const;
        const glm::mat4& GetModelMatrix() const;

        // --- Utility Methods ---

        void Translate(const glm::vec3& translation);
        void SetScale(const glm::vec3& newScale);
        void SetScale(float uniformScale);

    private:
        // storage
        glm::vec3 m_Position = glm::vec3(0.0f);
        glm::quat m_Rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f); // Identity quaternion
        glm::vec3 m_Scale    = glm::vec3(1.0f);

        mutable glm::mat4 m_View  = glm::mat4(1.0f);
        mutable glm::mat4 m_Model = glm::mat4(1.0f);

        mutable bool m_ViewMatrixDirty  = true;
        mutable bool m_ModelMatrixDirty = true;

        // cache for Euler angles to avoid conversion costs
        mutable glm::vec3 m_CachedEuler = glm::vec3(0.0f);
        mutable bool m_EulerDirty = true;
    };


}
