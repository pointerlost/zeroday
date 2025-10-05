//
// Created by pointerlost on 9/18/25.
//
#pragma once
#include <memory>
#include <utility>
#include "Core/UUID.h"
#include "Graphics/OpenGL/Camera/Camera.h"
#include "Graphics/OpenGL/Lighting/Light.h"
#include "Graphics/OpenGL/Material/material.h"
#include "Graphics/OpenGL/Transformations/Transformations.h"

namespace Zeroday {
    struct Light;
    struct MeshData3D;
    namespace opengl {
        struct Model;
        struct MaterialInstance;
    }
}

namespace Zeroday {

    struct TransformComponent {
        Transform m_Transform;

        TransformComponent(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale)
            : m_Transform(position, rotation, scale) {}
        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
    };

    struct IDComponent {
        UUID ID;

        IDComponent() = default;
        IDComponent(const IDComponent&) = default;
        explicit IDComponent(UUID id) : ID(id) {}
    };

    struct CameraComponent {
        Camera m_Camera;

        explicit CameraComponent(CameraMode mode) : m_Camera(mode) {}
        explicit CameraComponent(const Camera& cam) : m_Camera(cam) {}
        CameraComponent() = default;
        CameraComponent(const CameraComponent&) = default;
    };


    struct LightComponent {
        opengl::Light m_Light;

        [[nodiscard]] opengl::LightSSBO ToGPUFormat(const TransformComponent& transform) const {
            return m_Light.ToGPUFormat(transform);
        }

        // Helper methods for easy setup
        void SetAsDirectional(const glm::vec3& direction, const glm::vec3& color, float intensity) {
            m_Light = opengl::Light::CreateDirectional(direction, color, intensity);
        }

        void SetAsPoint(const glm::vec3& position, const glm::vec3& color, float intensity) {
            m_Light = opengl::Light::CreatePoint(position, color, intensity);
        }

        void SetAsSpot(const glm::vec3& position, const glm::vec3& direction,
                      const glm::vec3& color, float intensity, float innerAngle, float outerAngle) {
            m_Light = opengl::Light::CreateSpot(position, direction, color, intensity, innerAngle, outerAngle);
        }

        // Constructors
        explicit LightComponent(const opengl::Light& light) : m_Light(light) {}
        explicit LightComponent(opengl::LightType type) : m_Light(type) {}
        LightComponent() = default;
        LightComponent(const LightComponent&) = default;
    };

    struct MaterialComponent {
        std::shared_ptr<opengl::MaterialInstance> m_Instance;

        [[nodiscard]] opengl::MaterialSSBO ToGPUFormat() const;

        explicit MaterialComponent(std::shared_ptr<opengl::MaterialInstance> material) : m_Instance(std::move(material)) {}
        MaterialComponent() = default;
        MaterialComponent(const MaterialComponent&) = default;
    };

    struct MeshComponent {
        std::string m_SubMeshName = {};

        explicit MeshComponent(std::string subMeshName) : m_SubMeshName(std::move(subMeshName)) {}
        MeshComponent() = default;
        MeshComponent(const MeshComponent&) = default;
        MeshComponent& operator=(const MeshComponent&) = default;
    };

    struct ModelComponent {
        std::shared_ptr<opengl::Model> model;

        explicit ModelComponent(std::shared_ptr<opengl::Model> model) : model(std::move(model)) {}
        ModelComponent() = default;
        ModelComponent(const ModelComponent&) = default;
    };

    struct TagComponent {
        std::string tag = {};

        explicit TagComponent(std::string tag) : tag(std::move(tag)) {}
        TagComponent() = default;
        TagComponent(const TagComponent&) = default;

        explicit operator std::string() const { return tag; }
    };

}
