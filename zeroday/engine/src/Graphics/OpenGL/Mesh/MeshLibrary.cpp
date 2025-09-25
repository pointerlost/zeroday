//
// Created by pointerlost on 8/8/25.
//
#include "Graphics/OpenGL/Mesh/MeshLibrary.h"

#include "core/Base.h"
#include "Graphics/OpenGL/Mesh/MeshData3D.h"
#include "Graphics/OpenGL/Mesh/MeshFactory.h"

namespace Zeroday {

    MeshLibrary::MeshLibrary() : meshData3D(CreateRef<MeshData3D>()),
                                 meshFactory(CreateRef<MeshFactory>()) {
        meshFactory->SetupMeshes(meshData3D);
    }
}

