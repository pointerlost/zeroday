//
// Created by pointerlost on 8/8/25.
//
#pragma once
#include <memory>

namespace Zeroday {

    // forward declarations
    struct MeshData3D;
    class MeshFactory;

    /* *  MESH LIBRARY * */

    class MeshLibrary {
    public:
        MeshLibrary();

        [[nodiscard]] std::shared_ptr<MeshData3D> GetMeshData3D() const { return meshData3D; };

    private:
        std::shared_ptr<MeshData3D> meshData3D;
        std::shared_ptr<MeshFactory> meshFactory;
    };

}
