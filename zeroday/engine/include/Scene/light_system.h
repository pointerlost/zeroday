//
// Created by pointerlost on 9/16/25.
//
#pragma once
#include "systems.h"

namespace ecs::systems {

    class LightSystem final : public System<LightSystem> {
    public:
        void Update() override;
    };
}
