//
// Created by pointerlost on 8/15/25.
//
#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Graphics/OpenGL/Material/material.h"

namespace Zeroday::opengl {
    struct SubMeshInfo;
    struct MeshData3D;
}

namespace Zeroday::opengl {

    struct MeshEntry {
        Ref<MeshData3D> meshData;
        std::string subMeshName;
        Ref<MaterialInstance> material;
    };

    struct Model {
        std::vector<MeshEntry> meshes;

        void AddMesh(Ref<MeshData3D> data, const std::string& name, Ref<MaterialInstance> material) {
            meshes.push_back({std::move(data), name, std::move(material)});
        }
    };
}
