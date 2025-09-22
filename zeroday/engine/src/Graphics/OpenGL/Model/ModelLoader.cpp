//
// Created by pointerlost on 8/15/25.
//
#include "Graphics/OpenGL/Model/ModelLoader.h"
#include "core/Logger.h"
#include "Graphics/OpenGL/Material/material_lib.h"
#include "Graphics/OpenGL/Mesh/MeshData3D.h"
#include "Graphics/OpenGL/Model/Model.h"
#include "Graphics/OpenGL/Textures/TextureManager.h"

namespace Zeroday {

    // std::shared_ptr<Model> ModelLoader::load(const std::string &filePath) {
    //     Assimp::Importer importer;
    //
    //     const aiScene* scene = importer.ReadFile(filePath,
    //         aiProcess_Triangulate |
    //         aiProcess_FlipUVs |
    //         aiProcess_CalcTangentSpace |
    //         aiProcess_JoinIdenticalVertices);
    //
    //     if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
    //         Logger::error("Assimp failed to load model: " + std::string(importer.GetErrorString()));
    //         return nullptr;
    //     }
    //
    //     auto model = CreateRef<Model>();
    //
    //     std::string directory;
    //     size_t slashIndex = filePath.find_last_of("/\\");
    //     if (slashIndex != std::string::npos)
    //         directory = filePath.substr(0, slashIndex);
    //
    //     std::function<void(aiNode*, const aiScene* scene)> processNode;
    //     processNode = [&](aiNode* node, const aiScene* scene) {
    //         for (unsigned int i = 0; i < node->mNumMeshes; i++) {
    //             aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
    //             MeshEntry entry = processMesh(mesh, scene, directory);
    //             model->meshes.push_back(entry);
    //         }
    //
    //         // Process Children
    //         for (unsigned int i = 0; i < node->mNumChildren; i++) {
    //             processNode(node->mChildren[i], scene);
    //         }
    //     };
    //
    //     processNode(scene->mRootNode, scene);
    //
    //     return model;
    // }
    //
    // std::shared_ptr<Material> ModelLoader::processMaterial(aiMaterial *mat, const std::string &directory) {
    //     aiString name;
    //     mat->Get(AI_MATKEY_NAME, name);
    //
    //     // Check if this materail already exists
    //     auto existing = m_matLib->getMaterialByName(name.C_Str());
    //     if (existing) return existing;
    //
    //     auto material = CreateRef<Material>();
    //     material->m_name = name.C_Str();
    //
    //     aiColor3D ambient, diffuse, specular;
    //     float shininess = 32.0f;
    //
    //     mat->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
    //     mat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
    //     mat->Get(AI_MATKEY_COLOR_SPECULAR, specular);
    //     mat->Get(AI_MATKEY_SHININESS, shininess);
    //
    //     material->m_ambient = glm::vec3(ambient.r, ambient.g, ambient.b);
    //     material->m_diffuse = glm::vec3(diffuse.r, diffuse.g, diffuse.b);
    //     material->m_specular = glm::vec3(specular.r, specular.g, specular.b);
    //     material->m_shininess = shininess;
    //
    //     // Load Textures
    //     if (mat->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
    //         aiString texPath;
    //         mat->GetTexture(aiTextureType_DIFFUSE, 0, &texPath);
    //         std::string path = directory + "/" + texPath.C_Str();
    //         m_matLib->getTextureManager()->load(texPath.C_Str(), path);
    //         material->m_diffuseTexture = m_matLib->getTextureManager()->getTextureWithPath(texPath.C_Str());
    //     }
    //
    //     if (mat->GetTextureCount(aiTextureType_SPECULAR) > 0) {
    //         aiString texPath;
    //         mat->GetTexture(aiTextureType_SPECULAR, 0, &texPath);
    //         std::string path = directory + "/" + texPath.C_Str();
    //         m_matLib->getTextureManager()->load(texPath.C_Str(), path);
    //         material->m_specularTexture = m_matLib->getTextureManager()->getTextureWithPath(texPath.C_Str());
    //     }
    //
    //     // Add to library
    //     return material;
    // }
    //
    // MeshEntry ModelLoader::processMesh(aiMesh *mesh, const aiScene *scene, const std::string &directory) {
    //     std::vector<Vertex> vertices;
    //     std::vector<uint32_t> indices;
    //
    //     // Extract Vertices
    //     for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
    //         Vertex v;
    //         v.position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
    //         v.normal = mesh->HasNormals() ? glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z) : glm::vec3(0.0);
    //         if (mesh->mTextureCoords[0]) {
    //             v.texCoords = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
    //         } else {
    //             v.texCoords = glm::vec2(0.0);
    //         }
    //         vertices.push_back(v);
    //     }
    //
    //     // Extract Indices
    //     for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
    //         aiFace face = mesh->mFaces[i];
    //         for (unsigned int j = 0; j < face.mNumIndices; j++)
    //             indices.push_back(face.mIndices[j]);
    //     }
    //
    //     // Create MeshData3D
    //     auto meshData = CreateRef<MeshData3D>();
    //     SubMeshInfo& info = meshData->AddMesh3DToMeshData(mesh->mName.C_Str(), vertices, indices);
    //     meshData->uploadToGPU();
    //
    //     // Material
    //     std::shared_ptr<Material> material = nullptr;
    //     if (mesh->mMaterialIndex >= 0) {
    //         aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
    //         material = processMaterial(mat, directory);
    //     }
    //
    //     MeshEntry entry;
    //     entry.meshData    = meshData;
    //     entry.subMeshName = mesh->mName.C_Str();
    //     entry.material    = material;
    //
    //     return entry;
    // }
}
