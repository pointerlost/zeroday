//
// Created by pointerlost on 9/21/25.
//
#include "Scene/Scene.h"
#include "Scene/Components.h"
#include "Scene/Entity.h"

namespace Zeroday {

    Scene::Scene() {
    }

    Scene::~Scene() {
    }

    template<typename T>
    void Scene::OnComponentAdded(Entity entity, T& component)
    {
        static_assert(sizeof(T) == 0);
    }

    template<>
    void Scene::OnComponentAdded<Ecs::IDComponent>(Entity entity, Ecs::IDComponent& component)
    {
    }

    template<>
    void Scene::OnComponentAdded<Ecs::CameraComponent>(Entity entity, Ecs::CameraComponent& component)
    {
    }

    template<>
    void Scene::OnComponentAdded<Ecs::TransformComponent>(Entity entity, Ecs::TransformComponent& component)
    {
    }

    template<>
    void Scene::OnComponentAdded<Ecs::LightComponent>(Entity entity, Ecs::LightComponent& component)
    {
    }

    template<>
    void Scene::OnComponentAdded<Ecs::MaterialComponent>(Entity entity, Ecs::MaterialComponent& component)
    {
    }

    template<>
    void Scene::OnComponentAdded<Ecs::MeshComponent>(Entity entity, Ecs::MeshComponent& component)
    {
    }

    template<>
    void Scene::OnComponentAdded<Ecs::ModelComponent>(Entity entity, Ecs::ModelComponent& component)
    {
    }

    template<>
    void Scene::OnComponentAdded<Ecs::NameComponent>(Entity entity, Ecs::NameComponent& component)
    {
    }

    Entity Scene::CreateEntity(const std::string &name) {
        return CreateEntityWithUUID(UUID(), name);
    }

    Entity Scene::CreateEntityWithUUID(UUID uuid, const std::string& name) {
        Entity entity = { m_Registry.create(), this };
        entity.AddComponent<Ecs::IDComponent>(uuid);
        entity.AddComponent<Ecs::TransformComponent>();
        auto& tag = entity.AddComponent<Ecs::NameComponent>();
        tag.name = name.empty() ? "Entity" : name;

        m_Entities[uuid] = entity;

        return entity;
    }

    void Scene::DestroyEntity(Entity entity) {
        m_Entities.erase(entity.GetUUID());
        m_Registry.destroy(entity);
    }
}
