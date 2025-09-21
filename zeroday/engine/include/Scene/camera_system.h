//
// Created by pointerlost on 9/16/25.
//
#pragma once
#include "systems.h"

namespace ecs::systems {

    class CameraSystem final : public System<CameraSystem> {
    public:
        void Update() override;
    };
}
