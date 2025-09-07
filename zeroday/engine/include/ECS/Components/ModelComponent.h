//
// Created by pointerlost on 8/15/25.
//
#pragma once
#include <memory>

namespace Graphics { struct Model; }

namespace ECS {
    struct ModelComponent {
        std::shared_ptr<Graphics::Model> model;

        ModelComponent() = default;
        ModelComponent(const ModelComponent&) = default;
        ModelComponent(ModelComponent&&) = default;
        ModelComponent& operator=(const ModelComponent&) = default;
        ModelComponent& operator=(ModelComponent&&) = default;
    };
}
