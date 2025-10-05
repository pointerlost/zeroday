//
// Created by pointerlost on 8/15/25.
//
#include "Graphics/OpenGL/Model/ModelLoader.h"
#include <filesystem>
#include <format>

#include "Core/AssetManager.h"
#include "Core/File.h"
#include "Core/Logger.h"
#include "Core/Services.h"
#include "Graphics/OpenGL/Mesh/MeshData3D.h"
#include "Graphics/OpenGL/Model/Model.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Core/Assert.h"
#include "Graphics/OpenGL/Mesh/MeshLibrary.h"

Zeroday::File& fileMngr = Zeroday::File::Get();

namespace Zeroday::Graphics {

    std::shared_ptr<opengl::Model> ModelLoader::Load(const std::string &filePath) {
        Info(std::format("Loading Model: {}", filePath));

        if (!fileMngr.Exists(filePath)) {
            ZD_WARN(false, "Model file not found: {}", filePath);
            return nullptr;
        }

        // Reset state
        m_CurrentModel = CreateRef<opengl::Model>();
        m_CurrentModel->m_FilePath = filePath;
        m_CurrentModel->m_Name = std::filesystem::path(filePath).stem().string();
        m_CurrentDirectory = GetDirectoryFromPath(filePath);

        // Create assimp importer
        Assimp::Importer importer;

        // flags for complex models
        const unsigned int importFlags =
            aiProcess_Triangulate |
            aiProcess_GenSmoothNormals |
            aiProcess_FlipUVs |
            aiProcess_JoinIdenticalVertices |
            aiProcess_CalcTangentSpace |      // For normal mapping
            aiProcess_OptimizeMeshes |        // Important for large models
            aiProcess_ImproveCacheLocality;   // Better performance

        // Load the scene
        const aiScene* scene = importer.ReadFile(filePath, importFlags);

        // Check for errors
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            Warn(std::format("Failed to load model: {} - {}", filePath, importer.GetErrorString()));
            return nullptr;
        }

        Info(std::format("Model loaded: {} meshes, {} materials",
              scene->mNumMeshes, scene->mNumMaterials));

        // Start processing from root node
        ProcessNode(scene->mRootNode, scene, m_CurrentDirectory);

        Info(std::format("Model processing complete: {} submeshes", m_CurrentModel->m_Meshes.size()));

        return m_CurrentModel;
    }

    void ModelLoader::ProcessNode(aiNode *node, const aiScene *scene, const std::string &directory) {
        // Process all meshes in this node
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            opengl::MeshEntry meshEntry = ProcessMesh(mesh, scene, directory);
            m_CurrentModel->m_Meshes.push_back(std::move(meshEntry));
        }

        // Process all children nodes
        for (unsigned int i = 0; i < node->mNumChildren; i++) {
            ProcessNode(node->mChildren[i], scene, directory);
        }
    }

    opengl::MeshEntry ModelLoader::ProcessMesh(aiMesh *mesh, const aiScene *scene, const std::string &directory) {
        // Create containers for vertex data
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

        Info(std::format("Assimp Mesh Info: {} vertices, {} faces",
          mesh->mNumVertices, mesh->mNumFaces));

        // Process vertices
        vertices.reserve(mesh->mNumVertices);
        for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
            Vertex vertex{};

            // Position
            vertex.m_Position.x = mesh->mVertices[i].x;
            vertex.m_Position.y = mesh->mVertices[i].y;
            vertex.m_Position.z = mesh->mVertices[i].z;

            // Normal
            if (mesh->HasNormals()) {
                vertex.m_Normal.x = mesh->mNormals[i].x;
                vertex.m_Normal.y = mesh->mNormals[i].y;
                vertex.m_Normal.z = mesh->mNormals[i].z;
            } else {
                vertex.m_Normal = glm::vec3(0.0, 1.0, 0.0);
            }

            // Texture coordinates
            if (mesh->mTextureCoords[0]) {
                vertex.m_UV.x = mesh->mTextureCoords[0][i].x;
                vertex.m_UV.y = mesh->mTextureCoords[0][i].y;
            } else {
                vertex.m_UV = glm::vec2(0.0, 0.0);
            }

            vertices.push_back(vertex);
        }

        Info(std::format("Our vertices array: {} vertices", vertices.size()));

        // Process indices
        for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++) {
                indices.push_back(face.mIndices[j]);
            }
        }

        Info(std::format("Our indices array: {} indices", indices.size()));

        // Create unique mesh name
        std::string meshName = m_CurrentModel->m_Name + "_" + mesh->mName.C_Str();
        if (meshName == m_CurrentModel->m_Name + "_") {
            meshName = m_CurrentModel->m_Name + "_Mesh_" + std::to_string(m_CurrentModel->m_Meshes.size());
        }

        // Add to global meshData3D and upload to GPU
        Services::GetMeshLibrary()->GetMeshData3D()->AddMesh3DToMeshData(meshName, vertices, indices);

        // Process Material
        Ref<opengl::MaterialInstance> materialInstance;
        if (mesh->mMaterialIndex >= 0) {
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
            auto baseMaterial = ProcessMaterial(material, scene, directory);
            materialInstance = CreateRef<opengl::MaterialInstance>();
            materialInstance->m_Base = baseMaterial;
        } else {
            materialInstance = Services::GetAssetManager()->CreateMaterialInstance("default");
        }

        Info(std::format("Mesh processed: {} ({} vertices, {} indices)",
              meshName, vertices.size(), indices.size()));

        return { meshName, materialInstance };
    }

    Ref<opengl::Material> ModelLoader::ProcessMaterial(aiMaterial *mat, const aiScene *scene,
                                                       const std::string &directory) {
        auto* assetManager = Services::GetAssetManager();
        auto material = CreateRef<opengl::Material>();

        aiString matName;
        if (mat->Get(AI_MATKEY_NAME, matName) == AI_SUCCESS) {
            Info(std::format("Loading Material: {}", matName.C_Str()));
        }

        // Get Base Color
        aiColor3D diffuseColor(1.0f, 1.0f, 1.0f);
        if (mat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor) == AI_SUCCESS) {
            material->baseColor = glm::vec4(diffuseColor.r, diffuseColor.g, diffuseColor.b, 1.0f);
        }

        // Get emissive color
        aiColor3D emissiveColor(0.0f, 0.0f, 0.0f);
        if (mat->Get(AI_MATKEY_COLOR_EMISSIVE, emissiveColor) == AI_SUCCESS) {
            material->emissive = glm::vec3(emissiveColor.r, emissiveColor.g, emissiveColor.b);
        }

        // Set default metallic/roughness
        material->metallic  = 0.0f;
        material->roughness = 1.0f;

        /******************************* TEXTURE LOADING ********************************/

        auto LoadTexture = [&](aiTextureType aiType, opengl::MaterialTextureType ourType,
                      const std::string& fallbackName = "") -> bool {
            if (mat->GetTextureCount(aiType) > 0) {
                aiString texturePath;
                aiTextureMapping mapping;
                unsigned int uvindex = 0;
                float blend = 1.0f;
                aiTextureOp op = aiTextureOp_Add;
                aiTextureMapMode mapmode = aiTextureMapMode_Wrap;

                // Use the full GetTexture function to get all parameters
                if (mat->GetTexture(aiType, 0, &texturePath, &mapping, &uvindex, &blend, &op, &mapmode) == AI_SUCCESS) {
                    std::string textureFile = texturePath.C_Str();

                    // Find the actual texture file
                    std::string fullPath = FindTextureFile(directory, textureFile);

                    if (!fullPath.empty()) {
                        // Load or get existing texture
                        auto texture = assetManager->GetTextureWithPath(fullPath);
                        if (!texture) {
                            std::string textureName = std::filesystem::path(textureFile).stem().string() +
                                                     "_" + GetTextureTypeName(aiType);
                            texture = assetManager->Load(textureName, fullPath);
                        }

                        if (texture) {
                            material->textures[ourType] = texture;
                            Info(std::format("✅ Loaded texture: {} -> {}",
                                  GetTextureTypeName(aiType), textureFile));
                            return true;
                        }
                    } else {
                        Warn(std::format("❌ Texture file not found: {}", textureFile));
                    }
                }
            }
            return false;
        };

        // Load different texture types
        LoadTexture(aiTextureType_DIFFUSE,   opengl::MaterialTextureType::BaseColor, "default_basecolor");
        LoadTexture(aiTextureType_NORMALS,   opengl::MaterialTextureType::Normal, "default_normal");
        LoadTexture(aiTextureType_HEIGHT,    opengl::MaterialTextureType::Normal, "default_normal"); // Often used for normals
        LoadTexture(aiTextureType_SPECULAR,  opengl::MaterialTextureType::Roughness, "default_roughness");
        LoadTexture(aiTextureType_SHININESS, opengl::MaterialTextureType::Roughness, "default_roughness");
        LoadTexture(aiTextureType_AMBIENT,   opengl::MaterialTextureType::AmbientOcclusion, "default_ao");
        LoadTexture(aiTextureType_LIGHTMAP,  opengl::MaterialTextureType::AmbientOcclusion, "default_ao");
        LoadTexture(aiTextureType_OPACITY,   opengl::MaterialTextureType::BaseColor, "default_basecolor");

        // For PBR materials (glTF)
        LoadTexture(aiTextureType_BASE_COLOR, opengl::MaterialTextureType::BaseColor, "default_basecolor");
        LoadTexture(aiTextureType_NORMAL_CAMERA, opengl::MaterialTextureType::Normal, "default_normal");
        LoadTexture(aiTextureType_METALNESS, opengl::MaterialTextureType::Roughness, "default_roughness");

        auto SetDefaultIfMissing = [&](opengl::MaterialTextureType type) {
            if (!material->textures.contains(type)) {
                material->textures[type] = Services::GetAssetManager()->GetDefaultTexture(type);
            }
        };

        SetDefaultIfMissing(opengl::MaterialTextureType::BaseColor);
        SetDefaultIfMissing(opengl::MaterialTextureType::Normal);
        SetDefaultIfMissing(opengl::MaterialTextureType::Roughness);
        SetDefaultIfMissing(opengl::MaterialTextureType::AmbientOcclusion);
        SetDefaultIfMissing(opengl::MaterialTextureType::Displacement);

        Info(std::format("Material '{}' loaded with {} textures",
              matName.C_Str(), material->textures.size()));

        return material;
    }

    std::string ModelLoader::GetDirectoryFromPath(const std::string &filePath) {
        size_t slashIndex = filePath.find_last_of("/\\");
        return (slashIndex != std::string::npos) ? filePath.substr(0, slashIndex) : "";
    }

    std::string ModelLoader::GetTextureTypeName(aiTextureType type) {
        switch (type) {
            case aiTextureType_DIFFUSE: return "Diffuse/BaseColor";
            case aiTextureType_SPECULAR: return "Specular";
            case aiTextureType_AMBIENT: return "Ambient/AO";
            case aiTextureType_EMISSIVE: return "Emissive";
            case aiTextureType_HEIGHT: return "Height/Normal";
            case aiTextureType_NORMALS: return "Normals";
            case aiTextureType_SHININESS: return "Shininess/Roughness";
            case aiTextureType_OPACITY: return "Opacity";
            case aiTextureType_DISPLACEMENT: return "Displacement";
            case aiTextureType_LIGHTMAP: return "Lightmap";
            case aiTextureType_REFLECTION: return "Reflection";
            case aiTextureType_BASE_COLOR: return "BaseColor";
            case aiTextureType_NORMAL_CAMERA: return "NormalCamera";
            case aiTextureType_EMISSION_COLOR: return "EmissionColor";
            case aiTextureType_METALNESS: return "Metalness";
            case aiTextureType_DIFFUSE_ROUGHNESS: return "Roughness";
            case aiTextureType_AMBIENT_OCCLUSION: return "AmbientOcclusion";
            default: return "Unknown";
        }
    }

    std::string ModelLoader::FindTextureFile(const std::string &directory, const std::string &texturePath) {
        std::filesystem::path path(texturePath);
        std::string filename = path.filename().string();  // Get just the filename
        std::string stem = path.stem().string();
        std::string extension = path.extension().string();

        // Common texture extensions to try
        std::vector<std::string> extensions = {".png", ".jpg", ".jpeg", ".tga", ".bmp", ".PNG", ".JPG", ".JPEG"};
        if (!extension.empty()) {
            extensions.insert(extensions.begin(), extension);
        }

        // Common subdirectories
        std::vector<std::string> subdirs = {
            "",                            // Same directory as model
            "textures/",                   // textures folder
            "Textures/",                   // Textures folder (capitalized)
            "images/",                     // images folder
            "Images/",                     // Images folder
            "tex/",                        // tex folder
            "Tex/",                        // Tex folder
            "material/",                   // material folder
            "Material/",                   // Material folder
            "glTF/",                       // glTF folder (Sponza specific)
            "glTF",                        // glTF folder without slash
            "source/",                     // source folder (house model)
            "source/housetest/",           // nested source folder
        };

        // First, try the exact path as provided
        std::string exactPath = directory + "/" + texturePath;
        if (fileMngr.Exists(exactPath)) {
            Info(std::format("Found texture at exact path: {}", exactPath));
            return exactPath;
        }

        // SPECIAL CASE: For Sponza with numbered texture files
        // If texturePath is just a number like "10381718147657362067.jpg"
        // Try to find it in common texture directories
        if (std::all_of(stem.begin(), stem.end(), ::isdigit)) {
            Info(std::format("Detected numbered texture file: {}", filename));

            for (const auto& subdir : subdirs) {
                for (const auto& ext : extensions) {
                    std::string testPath = directory + "/" + subdir + stem + ext;
                    if (fileMngr.Exists(testPath)) {
                        Info(std::format("Found numbered texture at: {}", testPath));
                        return testPath;
                    }

                    // Also try with the full filename
                    testPath = directory + "/" + subdir + filename;
                    if (fileMngr.Exists(testPath)) {
                        Info(std::format("Found numbered texture at: {}", testPath));
                        return testPath;
                    }
                }
            }
        }

        // Try different combinations of subdirectories and extensions
        for (const auto& ext : extensions) {
            for (const auto& subdir : subdirs) {
                // Try: subdir + stem + extension
                std::string testPath = directory + "/" + subdir + stem + ext;
                if (fileMngr.Exists(testPath)) {
                    Info(std::format("Found texture at: {}", testPath));
                    return testPath;
                }

                // Try: subdir + filename (with original extension)
                testPath = directory + "/" + subdir + filename;
                if (fileMngr.Exists(testPath)) {
                    Info(std::format("Found texture at: {}", testPath));
                    return testPath;
                }

                // Try: subdir + filename with different extension
                if (!ext.empty()) {
                    testPath = directory + "/" + subdir + stem + ext;
                    if (fileMngr.Exists(testPath)) {
                        Info(std::format("Found texture at: {}", testPath));
                        return testPath;
                    }
                }
            }
        }

        // Special case: if texturePath is already a full path but relative to model directory
        std::string fullTexturePath = directory + "/" + texturePath;
        if (fileMngr.Exists(fullTexturePath)) {
            Info(std::format("Found texture at full path: {}", fullTexturePath));
            return fullTexturePath;
        }

        // LAST RESORT: Search recursively in the directory
        std::string foundPath = SearchRecursively(directory, filename);
        if (!foundPath.empty()) {
            Info(std::format("Found texture recursively at: {}", foundPath));
            return foundPath;
        }

        Warn(std::format("Could not locate texture: {} in directory: {}", texturePath, directory));
        return "";
    }

    std::string ModelLoader::SearchRecursively(const std::string &directory, const std::string &filename) {
        try {
            for (const auto& entry : std::filesystem::recursive_directory_iterator(directory)) {
                if (entry.is_regular_file() && entry.path().filename() == filename) {
                    return entry.path().string();
                }
            }
        } catch (const std::filesystem::filesystem_error& e) {
            Warn(std::format("Filesystem error while searching for texture: {}", e.what()));
        }
        return "";
    }
}
