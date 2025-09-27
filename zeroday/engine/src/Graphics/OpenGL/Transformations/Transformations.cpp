#include "Graphics/OpenGL/Transformations/Transformations.h"


namespace Zeroday {

    void Transform::SetPosition(const glm::vec3 &position) {
        m_Position = position;
        m_WorldMatrixDirty = true;
    }

    const glm::vec3& Transform::GetEulerRotation() const {
        if (m_EulerDirty) {
            m_CachedEuler = glm::degrees(glm::eulerAngles(m_Rotation));
            m_EulerDirty = false;
        }
        return m_CachedEuler;
    }

    void Transform::SetEulerRotation(const glm::vec3 &eulerDegrees) {
        const glm::vec3 eulerRad = glm::radians(eulerDegrees);
        m_Rotation   = glm::quat(eulerRad);
        m_EulerDirty = true;
        m_WorldMatrixDirty = true;
    }

    void Transform::Rotate(float angleDegrees, const glm::vec3 &axis) {
        const glm::quat rot = glm::angleAxis(glm::radians(angleDegrees), glm::normalize(axis));
        m_Rotation   = glm::normalize(rot * m_Rotation);
        m_EulerDirty = true;
        m_WorldMatrixDirty = true;
    }

    void Transform::RotateEuler(const glm::vec3 &eulerDegrees) {
        const auto rot = glm::quat(glm::radians(eulerDegrees));
        m_Rotation   = glm::normalize(rot * m_Rotation);
        m_EulerDirty = true;
        m_WorldMatrixDirty = true;
    }

    void Transform::LookAt(const glm::vec3 &target, const glm::vec3 &up) {
        const glm::vec3 direction = glm::normalize(target - m_Position);
        m_Rotation = glm::quatLookAt(-direction, up);
        m_EulerDirty = true;
        m_WorldMatrixDirty = true;
    }

    void Transform::ReCalculateWorldMatrix() const {
        const glm::mat4 translation = glm::translate(glm::mat4(1.0f), m_Position);
        const glm::mat4 rotationMat = glm::mat4_cast(m_Rotation);
        const glm::mat4 scaleMat    = glm::scale(glm::mat4(1.0f), m_Scale);
        m_WorldMatrix = translation * rotationMat * scaleMat;
        m_WorldMatrixDirty = false;
    }

    const glm::mat4& Transform::GetWorldMatrix() const {
        if (m_WorldMatrixDirty) {
            ReCalculateWorldMatrix();
        }
        return m_WorldMatrix;
    }

    void Transform::Translate(const glm::vec3 &translation) {
        m_Position += translation;
        m_WorldMatrixDirty = true;
    }

    void Transform::SetScale(const glm::vec3 &newScale) {
        m_Scale = newScale;
        m_WorldMatrixDirty = true;
    }

    void Transform::SetScale(float uniformScale) {
        m_Scale = glm::vec3(uniformScale);
        m_WorldMatrixDirty = true;
    }

}
