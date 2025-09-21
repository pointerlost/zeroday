//
// Created by pointerlost on 9/19/25.
//
#pragma once
#include <iostream>

#include "core/Assert.h"
#include "entt/entt.hpp"

namespace Zeroday { class Scene; }

namespace Zeroday {

    class Entity {
    public:
        Entity() = default;
        Entity(entt::entity handle, Scene* scene);
        Entity(const Entity& other) = default;

        template<typename T, typename... Args>
        T& AddComponent(Args&&... args) {
            ZD_ASSERT(!H)
        }
    };
}
