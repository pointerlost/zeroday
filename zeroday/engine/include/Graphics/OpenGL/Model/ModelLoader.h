//
// Created by pointerlost on 8/15/25.
//
#pragma once
#include <memory>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <glm/ext.hpp>

#include "Model.h"

namespace Zeroday::opengl {
    struct MaterialInstance;
}

namespace Zeroday::Graphics {

    class ModelLoader {
    public:
        std::shared_ptr<opengl::Model> Load(const std::string& filePath);

    private:
        void ProcessNode(aiNode* node, const aiScene* scene, const std::string& directory);
        opengl::MeshEntry ProcessMesh(aiMesh* mesh, const aiScene* scene, const std::string& directory);
        Ref<opengl::Material> ProcessMaterial(aiMaterial* mat, const aiScene* scene, const std::string& directory);

        std::string GetDirectoryFromPath(const std::string& filePath);
        std::string GetTextureTypeName(aiTextureType type);
        std::string FindTextureFile(const std::string& directory, const std::string& texturePath);
        std::string SearchRecursively(const std::string& directory, const std::string& filename);

    private:
        Ref<opengl::Model> m_CurrentModel;
        std::string m_CurrentDirectory;
    };

}
