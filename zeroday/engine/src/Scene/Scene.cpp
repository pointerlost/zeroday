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
        if (m_Entities.contains(uuid)) {
            Warn("Entity already exists!");
            return m_Entities[uuid];
        }
        Entity entity = { m_Registry.create(), this };
        entity.AddComponent<IDComponent>(uuid);
        entity.AddComponent<TransformComponent>();
        entity.AddComponent<NameComponent>().name = name.empty() ? CreateUniqueName() : name;

        m_Entities[uuid] = entity;

        return entity;
    }

    void Scene::DestroyEntity(Entity entity) {
        m_Entities.erase(entity.GetUUID());
        m_Registry.destroy(entity);
    }

    Entity Scene::GetEntityWithUUID(UUID uuid) {
        if (m_Entities.contains(uuid)) {
            return m_Entities[uuid];
        }
        ZD_WARN("Entity does not exist! returning a new one!");
        Warn("Entity does not exist! returning a new one!");
        return Entity{};
    }

    std::string Scene::CreateUniqueName() {
        std::string readNumbers;
        static std::string lastUsedName = "Entity";
        const std::string name = lastUsedName;

        int i = static_cast<int>(name.size()) - 1;
        while (i >= 0 && std::isdigit(name[i])) {
            readNumbers.push_back(name[i]);
            --i;
        }

        std::string baseName = name.substr(0, i + 1);
        int number = readNumbers.empty() ? 1 : std::stoi(readNumbers) + 1;
        baseName += std::to_string(number);

        lastUsedName = baseName;

        return baseName;
    }
}
