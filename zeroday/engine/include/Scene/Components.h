//
// Created by pointerlost on 9/18/25.
//
#pragma once
#include <memory>
#include <utility>

#include "core/UUID.h"
#include "Graphics/OpenGL/Camera/Camera.h"
#include "Graphics/OpenGL/Transformations/Transformations.h"

namespace Zeroday {
    class Light;
    struct Model;
    struct MeshData3D;
    struct MaterialInstance;
}

namespace Zeroday::Ecs {

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

        explicit CameraComponent(const Camera& cam) : m_Camera(cam) {}
        CameraComponent() = default;
        CameraComponent(const CameraComponent&) = default;
    };


    struct LightComponent {
        std::shared_ptr<Light> m_light;

        explicit LightComponent(std::shared_ptr<Light> light) : m_light(std::move(light)) {}
        LightComponent() = default;
        LightComponent(const LightComponent&) = default;
    };

    struct MaterialComponent {
        std::shared_ptr<MaterialInstance> m_Instance;

        explicit MaterialComponent(std::shared_ptr<MaterialInstance> material) : m_Instance(std::move(material)) {}
        MaterialComponent() = default;
        MaterialComponent(const MaterialComponent&) = default;
    };

    struct MeshComponent {
        std::shared_ptr<MeshData3D> meshData = {};
        std::string subMeshName = {};

        explicit MeshComponent(std::shared_ptr<MeshData3D> data) : meshData(std::move(data)) {}
        MeshComponent() = default;
        MeshComponent(const MeshComponent&) = default;
        MeshComponent& operator=(const MeshComponent&) = default;
    };

    struct ModelComponent {
        std::shared_ptr<Model> model;

        explicit ModelComponent(std::shared_ptr<Model> model) : model(std::move(model)) {}
        ModelComponent() = default;
        ModelComponent(const ModelComponent&) = default;
    };

    struct NameComponent {
        std::string name = {};

        explicit NameComponent(std::string name) : name(std::move(name)) {}
        NameComponent() = default;
        NameComponent(const NameComponent&) = default;

        explicit operator std::string() const { return name; }
    };


}
