//
// Created by pointerlost on 8/6/25.
//
#pragma once
#include <unordered_map>
#include "Entity.h"
#include <functional>

namespace ECS {

    enum class ComponentTypes {
        Transform,
        Camera,
        Mesh,
        Material,
        Light,
        Name,
    };

    template<typename T>
    class ComponentStorage {
    public:
        using Callback = std::function<void(Entity, std::remove_reference_t<T>*)>;

        void Add(Entity e, T comp) {
            components[e] = std::move(comp);
            if (onCreate) onCreate(e, &components[e]);
        }

        void Remove(Entity e) {
            if (components.contains(e)) {
                if (onDestroy) onDestroy(e, &components[e]);
                components.erase(e);
            }
        }

        [[nodiscard]] T *Get(Entity e) {
            auto it = components.find(e);
            return it != components.end() ? &it->second : nullptr;
        }
        [[nodiscard]] auto &GetAll() { return components; };

        void SetOnCreate(Callback cb)  { onCreate  = std::move(cb); }
        void SetOnDestroy(Callback cb) { onDestroy = std::move(cb); }

        [[nodiscard]] bool HasCallbacks() const {
            return onCreate || onDestroy;
        }

    private:
        std::unordered_map<Entity, T> components;
        Callback onCreate;
        Callback onDestroy;
    };

}
