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
        MeshData3D* meshData;
        std::string subMeshName;
        std::shared_ptr<MaterialInstance> material;
    };

    struct Model {
        std::vector<MeshEntry> meshes;

        void addMesh(MeshData3D* data, const std::string& name, std::shared_ptr<MaterialInstance> material) {
            meshes.push_back( {data, name, std::move(material) } );
        }
    };
}
