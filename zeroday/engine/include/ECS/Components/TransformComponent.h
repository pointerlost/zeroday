//
// Created by pointerlost on 8/7/25.
//
#pragma once
#include <memory>

class Transform;

namespace ECS {
    
    struct TransformComponent {
        std::shared_ptr<Transform> transform;

        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent& operator=(const TransformComponent&) = default;
        TransformComponent(TransformComponent&&) = default;
        TransformComponent& operator=(TransformComponent&&) = default;
    };
}
