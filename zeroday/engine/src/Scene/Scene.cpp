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
    void Scene::OnComponentAdded<IDComponent>(Entity entity, IDComponent& component)
    {
    }

    template<>
    void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
    {
    }

    template<>
    void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
    {
    }

    template<>
    void Scene::OnComponentAdded<LightComponent>(Entity entity, LightComponent& component)
    {
    }

    template<>
    void Scene::OnComponentAdded<MaterialComponent>(Entity entity, MaterialComponent& component)
    {
    }

    template<>
    void Scene::OnComponentAdded<MeshComponent>(Entity entity, MeshComponent& component)
    {
    }

    template<>
    void Scene::OnComponentAdded<ModelComponent>(Entity entity, ModelComponent& component)
    {
    }

    template<>
    void Scene::OnComponentAdded<NameComponent>(Entity entity, NameComponent& component)
    {
    }

    Entity Scene::CreateEntity(const std::string &name) {
        return CreateEntityWithUUID(UUID(), name);
    }

    Entity Scene::CreateEntityWithUUID(UUID uuid, const std::string& name) {
        Entity entity = { m_Registry.create(), this };
        entity.AddComponent<IDComponent>(uuid);
        entity.AddComponent<TransformComponent>();
        auto& tag = entity.AddComponent<NameComponent>();
        tag.name = name.empty() ? "Entity" : name;

        m_Entities[uuid] = entity;

        return entity;
    }

    void Scene::DestroyEntity(Entity entity) {
        m_Entities.erase(entity.GetUUID());
        m_Registry.destroy(entity);
    }

}
