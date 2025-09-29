//
// Created by pointerlost on 8/15/25.
//
#include "Graphics/OpenGL/Model/ModelLoader.h"
#include "core/Logger.h"
#include "core/Services.h"
#include "Graphics/OpenGL/Material/material_lib.h"
#include "Graphics/OpenGL/Mesh/MeshData3D.h"
#include "Graphics/OpenGL/Model/Model.h"
#include "Graphics/OpenGL/Textures/TextureManager.h"

namespace Zeroday::Graphics {
    //
    // std::shared_ptr<opengl::Model> ModelLoader::Load(const std::string &filePath) {
    //     Assimp::Importer importer;
    //     const aiScene* scene = importer.ReadFile(filePath,
    //         aiProcess_Triangulate | aiProcess_FlipUVs |
    //         aiProcess_CalcTangentSpace | aiProcess_JoinIdenticalVertices);
    //
    //     if (!scene || !scene->mRootNode) {
    //         Error("Assimp failed to load model: " + std::string(importer.GetErrorString()));
    //         return nullptr;
    //     }
    //
    //     auto model = CreateRef<opengl::Model>();
    //     std::string directory = GetDirectoryFromPath(filePath);
    //
    //     // Process all meshes
    //     ProcessNode(scene->mRootNode, scene, directory, model.get());
    //
    //     return model;
    // }
    //
    // std::shared_ptr<opengl::Material> ModelLoader::ProcessMaterial(aiMaterial *mat, const std::string &directory) {
    //     aiString name;
    //     mat->Get(AI_MATKEY_NAME, name);
    //
    //     // Check if this materail already exists
    //     auto existing = m_matLib->getMaterialByName(name.C_Str());
    //     if (existing) return existing;
    //
    //     auto material = CreateRef<opengl::Material>();
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
    // opengl::MeshEntry ModelLoader::ProcessMesh(aiMesh *mesh, const aiScene *scene, const std::string &directory) {
    //     std::vector<Vertex> vertices;
    //     std::vector<uint32_t> indices;
    //
    //     // Extract vertices
    //     for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
    //         Vertex v{};
    //         v.m_Position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
    //         v.m_Normal = mesh->HasNormals() ?
    //             glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z) : glm::vec3(0.0f);
    //
    //         if (mesh->mTextureCoords[0]) {
    //             v.m_UV = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
    //         } else {
    //             v.m_UV = glm::vec2(0.0f);
    //         }
    //         vertices.push_back(v);
    //     }
    //
    //     // Extract indices
    //     for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
    //         aiFace face = mesh->mFaces[i];
    //         for (unsigned int j = 0; j < face.mNumIndices; j++)
    //             indices.push_back(face.mIndices[j]);
    //     }
    //
    //     // Create mesh data
    //     auto meshData = CreateRef<MeshData3D>();
    //     meshData->AddMesh3DToMeshData(mesh->mName.C_Str(), vertices, indices);
    //
    //     // Note: Don't call UploadToGPU() here - do it once for all meshes
    //     // Don't call CreateUniversalVAO() here either
    //
    //     // Create material (simplified - you'll need your material system)
    //     auto material = Services::GetMaterialLibrary()->GetDefaultMaterial();
    //
    //     opengl::MeshEntry entry;
    //     entry.meshData = meshData;
    //     entry.subMeshName = mesh->mName.C_Str();
    //     entry.material = material;
    //
    //     return entry;
    // }
    //
    // void ModelLoader::ProcessNode(aiNode* node, const aiScene* scene, const std::string& directory, opengl::Model* model) {
    //     // Process meshes in current node
    //     for (unsigned int i = 0; i < node->mNumMeshes; i++) {
    //         aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
    //         opengl::MeshEntry entry = ProcessMesh(mesh, scene, directory);
    //         model->meshes.push_back(entry);
    //     }
    //
    //     // Process children
    //     for (unsigned int i = 0; i < node->mNumChildren; i++) {
    //         ProcessNode(node->mChildren[i], scene, directory, model);
    //     }
    // }
    //
    //
    //
    // std::string ModelLoader::GetDirectoryFromPath(const std::string& filePath) {
    //     size_t slashIndex = filePath.find_last_of("/\\");
    //     return (slashIndex != std::string::npos) ? filePath.substr(0, slashIndex) : "";
    // }
}
