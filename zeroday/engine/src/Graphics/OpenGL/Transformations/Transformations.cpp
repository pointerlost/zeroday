#include "Graphics/OpenGL/Transformations/Transformations.h"


namespace Zeroday {

    const glm::vec3 & Transform::GetEulerRotation() const {
        if (m_EulerDirty) {
            m_CachedEuler = glm::degrees(glm::eulerAngles(m_Rotation));
            m_EulerDirty = false;
        }
        return m_CachedEuler;
    }

    void Transform::SetEulerRotation(const glm::vec3 &eulerDegrees) {
        const glm::vec3 eulerRad = glm::radians(eulerDegrees);
        m_Rotation = glm::quat(eulerRad);
        m_EulerDirty = true;
        m_ViewMatrixDirty  = true;
        m_ModelMatrixDirty = true;
    }

    void Transform::Rotate(float angleDegrees, const glm::vec3 &axis) {
        const glm::quat rot = glm::angleAxis(glm::radians(angleDegrees), glm::normalize(axis));
        m_Rotation   = glm::normalize(rot * m_Rotation);
        m_EulerDirty = true;
        m_ViewMatrixDirty  = true;
        m_ModelMatrixDirty = true;
    }

    void Transform::RotateEuler(const glm::vec3 &eulerDegrees) {
        const auto rot = glm::quat(glm::radians(eulerDegrees));
        m_Rotation   = glm::normalize(rot * m_Rotation);
        m_EulerDirty = true;
        m_ViewMatrixDirty  = true;
        m_ModelMatrixDirty = true;
    }

    void Transform::LookAt(const glm::vec3 &target, const glm::vec3 &up) {
        const glm::vec3 direction = glm::normalize(target - m_Position);
        m_Rotation   = glm::quatLookAt(direction, up);
        m_EulerDirty = true;
        m_ViewMatrixDirty  = true;
    }

    void Transform::ReCalculateViewMatrix() const {
        const glm::mat4 rotationMat    = glm::mat4_cast(m_Rotation);
        const glm::mat4 translationMat = glm::translate(glm::mat4(1.0f), -m_Position);
        m_View = rotationMat * translationMat;
        m_ViewMatrixDirty = false;
    }

    void Transform::ReCalculateModelMatrix() const {
        const glm::mat4 translation = glm::translate(glm::mat4(1.0f), m_Position);
        const glm::mat4 rotationMat = glm::mat4_cast(m_Rotation);
        const glm::mat4 scaleMat    = glm::scale(glm::mat4(1.0f), m_Scale);
        m_Model = translation * rotationMat * scaleMat;
        m_ModelMatrixDirty = false;
    }

    const glm::mat4& Transform::GetViewMatrix() const {
        if (m_ViewMatrixDirty) {
            ReCalculateViewMatrix();
        }
        return m_View;
    }

    const glm::mat4& Transform::GetModelMatrix() const {
        if (m_ModelMatrixDirty) {
            ReCalculateModelMatrix();
        }
        return m_Model;
    }

    void Transform::Translate(const glm::vec3 &translation) {
        m_Position += translation;
        m_ViewMatrixDirty = true;
    }

    void Transform::SetScale(const glm::vec3 &newScale) {
        m_Scale = newScale;
        m_ViewMatrixDirty  = true;
        m_ModelMatrixDirty = true;
    }

    void Transform::SetScale(float uniformScale) {
        m_Scale = glm::vec3(uniformScale);
        m_ViewMatrixDirty  = true;
        m_ModelMatrixDirty = true;
    }

}
