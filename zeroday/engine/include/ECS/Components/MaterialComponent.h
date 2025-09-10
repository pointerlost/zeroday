//
// Created by pointerlost on 8/8/25.
//
#pragma once
#include <memory>

namespace Graphics { struct MaterialInstance; }

namespace ECS {
    
    struct MaterialComponent {
        std::shared_ptr<Graphics::MaterialInstance> instance;

        // Default initializer for instance
        MaterialComponent();
    };
}
