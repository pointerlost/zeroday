//
// Created by pointerlost on 8/8/25.
//
#include "Graphics/OpenGL/Mesh/MeshLibrary.h"
#include "Graphics/OpenGL/Mesh/MeshData3D.h"
#include "Graphics/OpenGL/Mesh/MeshFactory.h"

namespace Zeroday {

    MeshLibrary::MeshLibrary() : meshData3D(std::make_shared<MeshData3D>()),
                                 meshFactory(std::make_shared<MeshFactory>()) {
        meshFactory->setupMeshes(meshData3D);
    }
}

