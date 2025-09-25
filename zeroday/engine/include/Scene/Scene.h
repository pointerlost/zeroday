//
// Created by pointerlost on 9/18/25.
//
#pragma once
#include <iostream>
#include <unordered_map>
#include "core/UUID.h"
#include "entt/entt.hpp"

namespace Zeroday { class Entity; }

namespace Zeroday {

    class Scene {
    public:
        Scene();
        ~Scene();

        Entity CreateEntity(const std::string& name = std::string());
        Entity CreateEntityWithUUID(UUID uuid, const std::string& name = std::string());
        void DestroyEntity(Entity entity);

        template<typename... Components>
        auto GetAllEntitiesWith() {
            return m_Registry.view<Components...>();
        }

        Entity GetEntityWithUUID(UUID uuid);
        size_t GetEntityCount() const { return m_Entities.size(); }

    private:
        template <typename T>
        void OnComponentAdded(Entity entity, T& component);

        std::string CreateUniqueName();
    private:
        entt::registry m_Registry;
        std::unordered_map<UUID ,Entity> m_Entities;

        friend class Entity;
    };
}
