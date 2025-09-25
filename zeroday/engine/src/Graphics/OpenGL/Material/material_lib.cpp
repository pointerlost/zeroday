#include "Graphics/OpenGL/Material/material_lib.h"

#include "core/Base.h"
#include "core/File.h"
#include "Graphics/OpenGL/Textures/TextureManager.h"
#include "core/Logger.h"
#include "core/Services.h"
#include "Graphics/OpenGL/GPU_buffers.h"
#define DEBUG_PTR(ptr) DEBUG::DebugForEngineObjectPointers(ptr)

static auto& file = Zeroday::File::get();

namespace Zeroday {

    bool MaterialLibrary::CreateMaterials(const std::string& filePath) {
        std::string content = file.readFromFile(filePath);
        if (content.empty()) {
            Warn("[MaterialLibrary::CreateMaterials] Content is empty for file: " + filePath + "\n");
            return false;
        }

        json parsed;
        try {
            parsed = json::parse(content);
        } catch (const json::parse_error& e) {
            Warn("[MaterialLibrary::CreateMaterials] JSON parse error: " + std::string(e.what()) + "\n");
            return false;
        }

        if (!parsed.contains("materials") || !parsed["materials"].is_array()) {
            Warn("[MaterialLibrary::CreateMaterials] Invalid material file structure! Missing 'materials' array.\n");
            return false;
        }

        int createdCount = 0;
        for (auto& mat : parsed["materials"]) {
            if (!mat.contains("name") || !mat["name"].is_string()) {
                Warn("[MaterialLibrary::CreateMaterials] Material missing name, skipping...");
                continue;
            }

            std::string name = mat["name"].get<std::string>();

            // Fallback values
            glm::vec4 baseColor(1.0f);
            glm::vec3 emissive(0.0f);
            float metallic = 0.0f;
            float roughness = 1.0f;

            // Parse baseColor with validation
            if (mat.contains("baseColor")) {
                if (mat["baseColor"].is_array() && mat["baseColor"].size() >= 3) {
                    try {
                        baseColor.r = mat["baseColor"][0].get<float>();
                        baseColor.g = mat["baseColor"][1].get<float>();
                        baseColor.b = mat["baseColor"][2].get<float>();
                        baseColor.a = (mat["baseColor"].size() >= 4) ? mat["baseColor"][3].get<float>() : 1.0f;
                    } catch (const json::exception& e) {
                        Warn("[MaterialLibrary::CreateMaterials] Invalid baseColor for material '" + name + "', using default.");
                    }
                }
            }

            // Parse emissive with validation
            if (mat.contains("emissive") && mat["emissive"].is_array() && mat["emissive"].size() == 3) {
                try {
                    emissive.r = mat["emissive"][0].get<float>();
                    emissive.g = mat["emissive"][1].get<float>();
                    emissive.b = mat["emissive"][2].get<float>();
                } catch (const json::exception& e) {
                    Warn("[MaterialLibrary::CreateMaterials] Invalid emissive for material '" + name + "', using default.");
                }
            }

            // Parse metallic and roughness
            if (mat.contains("metallic")) {
                try {
                    metallic = std::clamp(mat["metallic"].get<float>(), 0.0f, 1.0f);
                } catch (const json::exception& e) {
                    Warn("[MaterialLibrary::CreateMaterials] Invalid metallic for material '" + name + "', using default.");
                }
            }

            if (mat.contains("roughness")) {
                try {
                    roughness = std::clamp(mat["roughness"].get<float>(), 0.0f, 1.0f);
                } catch (const json::exception& e) {
                    Warn("[MaterialLibrary::CreateMaterials] Invalid roughness for material '" + name + "', using default.");
                }
            }

            auto material = CreateRef<opengl::Material>();
            material->name = name;
            material->baseColor = baseColor;
            material->emissive = emissive;
            material->metallic = metallic;
            material->roughness = roughness;

            // Add default textures
            auto textureManager = Services::GetTextureManager();
            if (textureManager) {
                material->textures[opengl::MaterialTextureType::BaseColor] = textureManager->GetDefaultTexture(opengl::MaterialTextureType::BaseColor);
                material->textures[opengl::MaterialTextureType::Normal]    = textureManager->GetDefaultTexture(opengl::MaterialTextureType::Normal);
                material->textures[opengl::MaterialTextureType::Roughness] = textureManager->GetDefaultTexture(opengl::MaterialTextureType::Roughness);
                material->textures[opengl::MaterialTextureType::Metallic]  = textureManager->GetDefaultTexture(opengl::MaterialTextureType::Metallic);
            }

            m_materials[name] = material;
            createdCount++;

            Info("Created material: " + name);
        }

        Info("[MaterialLibrary::CreateMaterials] Successfully created " + std::to_string(createdCount) + " materials from " + filePath);
        return createdCount > 0;
    }

    Ref<opengl::Material> MaterialLibrary::GetMaterialByName(const std::string &name) {
        const auto it = m_materials.find(name);
        if (it == m_materials.end()) {
            Warn("Material '" + name + "' not found! Returning default material.\n");
            return GetDefaultMaterial();
        }
        return it->second;
    }

    Ref<opengl::Material> MaterialLibrary::GetDefaultMaterial() const {
        auto textureManager = Services::GetTextureManager();

        static Ref<opengl::Material> fallback = [textureManager]{
            auto mat = CreateRef<opengl::Material>();
            mat->name      = "default";
            mat->baseColor = glm::vec4(1.0f);
            mat->emissive  = glm::vec3(0.0f);
            mat->metallic  = 0.0f;
            mat->roughness = 1.0f;

            mat->textures[opengl::MaterialTextureType::BaseColor]        = textureManager->GetDefaultTexture(opengl::MaterialTextureType::BaseColor);
            mat->textures[opengl::MaterialTextureType::Normal]           = textureManager->GetDefaultTexture(opengl::MaterialTextureType::Normal);
            mat->textures[opengl::MaterialTextureType::Roughness]        = textureManager->GetDefaultTexture(opengl::MaterialTextureType::Roughness);
            mat->textures[opengl::MaterialTextureType::Metallic]         = textureManager->GetDefaultTexture(opengl::MaterialTextureType::Metallic);
            mat->textures[opengl::MaterialTextureType::AmbientOcclusion] = textureManager->GetDefaultTexture(opengl::MaterialTextureType::AmbientOcclusion);
            mat->textures[opengl::MaterialTextureType::Emissive]         = textureManager->GetDefaultTexture(opengl::MaterialTextureType::Emissive);

            Info("Default Material added!");
            return mat;
        }();
        return fallback;
    }

    Ref<opengl::MaterialInstance> MaterialLibrary::CreateInstance(const std::string &name) {
        const auto base = GetMaterialByName(name);
        auto instance  = CreateRef<opengl::MaterialInstance>();
        instance->m_Base = base;
        return instance;
    }

}



