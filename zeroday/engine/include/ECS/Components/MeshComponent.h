//
// Created by pointerlost on 8/6/25.
//
#pragma once
#include <memory>

// add include for mesh data 3d if needed

namespace Graphics {
    struct MeshData3D;
}

namespace ECS {
    struct MeshComponent {
        std::shared_ptr<Graphics::MeshData3D> meshData = {};
        std::string subMeshName = {};

        MeshComponent() = default;
        MeshComponent(const MeshComponent&) = default;
        MeshComponent& operator=(const MeshComponent&) = default;
        MeshComponent(MeshComponent&&) = default;
        MeshComponent& operator=(MeshComponent&&) = default;
    };
}

