//
// Created by pointerlost on 9/16/25.
//
#pragma once
#include "systems.h"

namespace ecs { class World; }

namespace ecs::systems {

    class TransformSystem final : public System<TransformSystem> {
    public:
        void Update(World* world) override;
    };
}
