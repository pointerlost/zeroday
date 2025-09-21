//
// Created by pointerlost on 9/16/25.
//
#pragma once
#include "systems.h"

namespace ecs::systems {

    class MaterialSystem final : public System<MaterialSystem> {
    public:
        void Update() override;
    };
}
