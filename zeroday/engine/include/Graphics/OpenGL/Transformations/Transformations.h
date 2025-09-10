#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>


class Transform {
public:
    // Get Euler angles (degrees) - user friendly (for example UI)
    glm::vec3 GetEulerRotation() const {
        if (m_eulerDirty) {
            m_cachedEuler = glm::degrees(glm::eulerAngles(rotation));
            m_eulerDirty = false;
        }
        return m_cachedEuler;
    }

    // set Euler angles (degrees) - converts to quaternion internally
    void SetEulerRotation(const glm::vec3& eulerDegrees) {
        const glm::vec3 eulerRad = glm::radians(eulerDegrees);
        rotation = glm::quat(eulerRad);
        m_eulerDirty = true;
    }

    // set individual Euler components
    void SetEulerRotation(float x, float y, float z) {
        SetEulerRotation(glm::vec3(x, y, z));
    }

    // --- Quaternion Interface (for internal use) ---

    // get raw quaternion
    const glm::quat& GetRotation() const { return rotation; }

    // set quaternion directly
    void SetRotation(const glm::quat& newRotation) {
        rotation = glm::normalize(newRotation);
        m_eulerDirty = true;
    }

    // --- Rotation Operations ---

    // rotate around axis (quaternion operation)
    void Rotate(float angleDegrees, const glm::vec3& axis) {
        const glm::quat rot = glm::angleAxis(glm::radians(angleDegrees), glm::normalize(axis));
        rotation = glm::normalize(rot * rotation);
        m_eulerDirty = true;
    }

    // rotate using Euler angles (convenience method)
    void RotateEuler(const glm::vec3& eulerDegrees) {
        const auto rot = glm::quat(glm::radians(eulerDegrees));
        rotation       = glm::normalize(rot * rotation);
        m_eulerDirty = true;
    }

    // look at target point
    void LookAt(const glm::vec3& target, const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f)) {
        const glm::vec3 direction = glm::normalize(target - position);
        rotation = glm::quatLookAt(direction, up);
        m_eulerDirty = true;
    }

    // --- Direction Vectors ---

    glm::vec3 GetForward() const { return rotation * glm::vec3(0.0f, 0.0f, -1.0f); }
    glm::vec3 GetRight()   const { return rotation * glm::vec3(1.0f, 0.0f, 0.0f);  }
    glm::vec3 GetUp()      const { return rotation * glm::vec3(0.0f, 1.0f, 0.0f);  }

    // --- Transformation Matrices ---

    glm::mat4 GetModelMatrix() const {
        const glm::mat4 translation = glm::translate(glm::mat4(1.0f), position);
        const glm::mat4 rotationMat = glm::mat4_cast(rotation);
        const glm::mat4 scaleMat    = glm::scale(glm::mat4(1.0f), scale);
        return translation * rotationMat * scaleMat;
    }

    glm::mat4 GetViewMatrix() const {
        const glm::mat4 rotationMat    = glm::mat4_cast(rotation);
        const glm::mat4 translationMat = glm::translate(glm::mat4(1.0f), -position);
        return rotationMat * translationMat;
    }

    // --- Utility Methods ---

    void Translate(const glm::vec3& translation) {
        position += translation;
    }

    void SetScale(const glm::vec3& newScale) {
        scale = newScale;
    }

    void SetScale(float uniformScale) {
        scale = glm::vec3(uniformScale);
    }

private:
    // storage
    glm::vec3 position = glm::vec3(0.0f);
    glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f); // Identity quaternion
    glm::vec3 scale    = glm::vec3(1.0f);

    // cache for Euler angles to avoid conversion costs
    mutable glm::vec3 m_cachedEuler = glm::vec3(0.0f);
    mutable bool m_eulerDirty = true;
};

