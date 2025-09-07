//
// Created by pointerlost on 8/8/25.
//
#pragma once
#include <memory>

namespace LIGHTING { class Light; }

namespace ECS {
    struct LightComponent {
        std::shared_ptr<LIGHTING::Light> light;

        LightComponent() = default;
        LightComponent(const LightComponent&) = default;
        LightComponent(LightComponent&&) = default;
        LightComponent& operator=(const LightComponent&) = default;
        LightComponent& operator=(LightComponent&&) = default;
    };
}
