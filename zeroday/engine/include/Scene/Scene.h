//
// Created by pointerlost on 9/18/25.
//
#pragma once
#include <iostream>
#include <unordered_map>
#include "entt/entt.hpp"
#include "core/UUID.h"


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


    private:
        template <typename T>
        void OnComponentAdded(Entity entity, T& component);

    private:
        entt::registry m_Registry;

        std::unordered_map<UUID ,entt::entity> m_Entities;

        friend class Entity;
    };
}
