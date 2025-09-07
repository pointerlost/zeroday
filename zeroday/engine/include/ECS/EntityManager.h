//
// Created by pointerlost on 9/4/25.
//
#pragma once
#include <unordered_map>
#include <vector>
#include <iostream>
#include <queue>
#include "Entity.h"

namespace ECS {

    class EntityManager {
    public:
        EntityManager() = default;
        ~EntityManager() = default;

        static Entity Create();
        static EntityInfo& GetEntityInfo(Entity e)      { return entityInfos[e]; }
        static const std::vector<Entity>& GetEntities() { return entities;       }

        static void Remove(Entity e);
        static void CleanDeletedEntities();

        static void SetMobility(Entity e, EntityMobility mobility) { entityInfos[e].mobility = mobility; }
        static EntityMobility GetMobility(Entity e)                { return entityInfos[e].mobility;     }

    private:
        inline static std::unordered_map<Entity, EntityInfo> entityInfos;
        inline static std::vector<Entity> entities;
        inline static std::unordered_map<Entity, uint32_t> hashEntitiesIndex;
        inline static std::queue<Entity> freeEntities;
        // when deleted an entity middle of the frame to avoid crashes add these entities into a queue
        inline static std::vector<Entity> needsCleaningEntities;

        inline static void CleanEntityProperties(Entity e);
    };
}