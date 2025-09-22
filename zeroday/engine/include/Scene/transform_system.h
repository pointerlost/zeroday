//
// Created by pointerlost on 9/16/25.
//
#pragma once
#include "systems.h"

namespace ecs { class Scene; }

namespace ecs::systems {

    class TransformSystem final : public System<TransformSystem> {
    public:
        void Update(Scene* world) override;
    };
}
