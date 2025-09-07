//
// Created by pointerlost on 8/6/25.
//
#pragma once
#include <memory>
#include <vector>
#include <typeindex>
#include <unordered_set>
#include "ComponentStorage.h"
#include "component_registry.h"
#include "Entity.h"
#include "EntityManager.h"

namespace ECS {

    class World {
    public:

        Entity CreateEntity()               { return EntityManager::Create();         }
        void   RemoveEntity(Entity entity)  { EntityManager::Remove(entity);          }
        void   CleanResources()             { EntityManager::CleanDeletedEntities();  }
        EntityInfo& GetEntityInfo(Entity e) { return EntityManager::GetEntityInfo(e); }
        const std::vector<Entity>& GetEntities()   const          { return EntityManager::GetEntities();     }
        void SetEntityMobility(Entity e, EntityMobility mobility) { EntityManager::SetMobility(e, mobility); }
        EntityMobility GetEntityMobility(Entity e) const          { return EntityManager::GetMobility(e);    }

        template<typename T>
        void AddComponent(Entity e, T&& c) {
            auto& storage = getStorage<T>();
            storage.Add(e, std::forward<T>(c));
        }

        void RemoveComponent(Entity e);

        template<typename T>
        [[nodiscard]] T* GetComponent(Entity e) { return getStorage<T>().Get(e); }

        template<typename T>
        [[nodiscard]] ComponentStorage<T>& getStorage() {
            static ComponentStorage<T> storage;

            if (!storage.HasCallbacks()) {
                storage.SetOnCreate([this](Entity e, std::remove_reference_t<T>* comp) {
                    // When a component is created in storage, register it
                    GetRegistry<T>().Add(e, *comp);
                });
                storage.SetOnDestroy([this](Entity e, std::remove_reference_t<T>* comp) {
                    // When a component is destroyed in storage, remove from registry
                    GetRegistry<T>().Remove(e);
                });
            }

            return storage;
        }

        // We have to get registry with specifying state, like editor state or game state
        // purpose: storing buffer's properties of some components | dirty tracking, buffer updating etc.
        // it definitely does not mean the same thing with (ComponentStorage)!!
        template<typename T>
        REGISTRY::ComponentRegistry<T>& GetRegistry() {
            // to avoid requiring a complete T
            // Using T* instead of T allows incomplete types
            const auto key = std::type_index(typeid(T*));
            auto it = registries.find(key);

            if (it == registries.end()) {
                auto holder = std::make_unique<REGISTRY::RegistryHolder<T>>(*this);
                auto& ref = *holder->registry;
                registries.emplace(key, std::move(holder));
                return ref;
            }

            return *static_cast<REGISTRY::RegistryHolder<T>*>(it->second.get())->registry;
        }

    private:
        std::unordered_map<std::type_index, std::unique_ptr<REGISTRY::IRegistry>> registries;
    };

}


