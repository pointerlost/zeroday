//
// Created by pointerlost on 8/15/25.
//
#pragma once
#include <memory>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <glm/ext.hpp>

namespace Graphics
{
    class MeshLibrary;
    class MaterialLibrary;
    struct MeshData3D;
    struct MaterialInstance;
    struct Material;
    struct Model;
    struct MeshEntry;
}

namespace Graphics {

    class ModelLoader {
    public:
        std::shared_ptr<Model> load(const std::string& filePath);

    private:
        std::shared_ptr<MaterialInstance> processMaterial(aiMaterial* mat, const std::string& directory);
        MeshEntry processMesh(aiMesh* mesh, const aiScene* scene,  const std::string& directory);

        float ShininessToRoughness(float shininess);
        float SpecularToMetallic(const glm::vec3& spec, float defaultF0 = 0.04f);
    };
}
