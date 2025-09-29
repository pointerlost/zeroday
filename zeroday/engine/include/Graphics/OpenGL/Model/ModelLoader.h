//
// Created by pointerlost on 8/15/25.
//
#pragma once
#include <memory>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <glm/ext.hpp>

#include "Model.h"

namespace Zeroday {
    namespace opengl {
        struct MaterialInstance;
    }
}

namespace Zeroday::Graphics {

    class ModelLoader {
    public:
        std::shared_ptr<opengl::Model> Load(const std::string& filePath);

    private:
        std::shared_ptr<opengl::Material> ProcessMaterial(aiMaterial* mat, const std::string& directory);
        opengl::MeshEntry ProcessMesh(aiMesh* mesh, const aiScene* scene,  const std::string& directory);
        void ProcessNode(aiNode *node, const aiScene *scene, const std::string &directory, opengl::Model *model);

        float ShininessToRoughness(float shininess);
        float SpecularToMetallic(const glm::vec3& spec, float defaultF0 = 0.04f);

        std::string GetDirectoryFromPath(const std::string &filePath);
    };
}
