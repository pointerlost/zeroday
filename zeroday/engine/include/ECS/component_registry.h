//
// Created by pointerlost on 8/25/25.
//
#pragma once
#include <memory>
#include <unordered_map>
#include "freelist_registry.h"

namespace ECS {
    struct ModelComponent;
    struct LightComponent;
    struct CameraComponent;
    struct MeshComponent;
    struct MaterialComponent;
    struct TransformComponent;
    struct NameComponent;
    class World;
}
namespace OpenGL { struct RenderState; }

namespace ECS::REGISTRY {

    struct IRegistry {
        virtual ~IRegistry() = default;
    };

    template<typename T>
    class ComponentRegistry {
    public:
        explicit ComponentRegistry(World& w) : world(w) {}

        uint32_t Add(Entity e, const T& comp);
        void     Remove(Entity e);

              T* GetByIndex(uint32_t idx)       { return store.get(idx); } // access directly from freelist
        const T* GetByIndex(uint32_t idx) const { return store.get(idx); }

        Entity getEntityByIndex(uint32_t idx) {
            const auto it = indexToEntity.find(idx);
            return it != indexToEntity.end() ? it->second : UINT32_MAX;
        }

        T* get(Entity e) {
            auto it = entityToIndex.find(e);
            if (it == entityToIndex.end()) return nullptr;
            return store.get(it->second);
        }

        size_t GetCount()    const { return store.GetActiveCount(); }
        size_t GetCapacity() const { return store.GetCapacity(); }

        uint32_t getIndex(Entity e) const {
            auto it = entityToIndex.find(e);
            if (it != entityToIndex.end()) return it->second;
            return UINT32_MAX;
        }

        std::vector<uint32_t>& getDirty(EntityMobility mobility) { return dirtyTracker.getDirtyVec(mobility); }
        const std::vector<uint32_t>& getDirty(EntityMobility mobility) const { return dirtyTracker.getDirtyVec(mobility); }
        void clearDirty(EntityMobility mobility) { dirtyTracker.clear(mobility); }
        void clearAllDirties() { dirtyTracker.clearAll(); }

    private:
        FreeList<T> store;
        DirtyTracker dirtyTracker;
        std::unordered_map<Entity, uint32_t> entityToIndex;
        std::unordered_map<uint32_t, Entity> indexToEntity;

        World& world;
    };

    template <typename T>
    struct RegistryHolder final : IRegistry {
        std::unique_ptr<ComponentRegistry<T>> registry;
        explicit RegistryHolder(World& world) : registry(std::make_unique<ComponentRegistry<T>>(world)) {}
    };

}

namespace ECS::REGISTRY {
    // Explicit template instantiations
    extern template class ComponentRegistry<NameComponent>;
    extern template class ComponentRegistry<TransformComponent>;
    extern template class ComponentRegistry<MaterialComponent>;
    extern template class ComponentRegistry<MeshComponent>;
    extern template class ComponentRegistry<CameraComponent>;
    extern template class ComponentRegistry<LightComponent>;
    extern template class ComponentRegistry<ModelComponent>;
}