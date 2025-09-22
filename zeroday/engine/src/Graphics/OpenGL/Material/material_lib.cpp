#include "Graphics/OpenGL/Material/material_lib.h"

#include "core/Base.h"
#include "core/File.h"
#include "Graphics/OpenGL/Textures/TextureManager.h"
#include "core/Logger.h"
#include "core/Services.h"
#include "Graphics/OpenGL/GPU_buffers.h"
#define DEBUG_PTR(ptr) DEBUG::DebugForEngineObjectPointers(ptr)

static auto& file = Zeroday::File::get();

namespace Zeroday
{
    bool MaterialLibrary::CreateMaterials(const std::string& filePath)
    {
        std::string content = file.readFromFile(filePath);
        if (content.empty()) {
            Warn("[MaterialLibrary::CreateMaterials] content is empty!\n");
            return false;
        }

        json parsed = json::parse(content);
        if (!parsed.contains("materials") || !parsed["materials"].is_array()) {
            Warn("[MaterialLibrary::CreateMaterials] Invalid material file structure!\n");
            return false;
        }

        for (auto& mat : parsed["materials"]) {
            std::string name = mat.value("name", "unnamed");

            // Fallback values
            glm::vec4 baseColor(1.0f);
            glm::vec3 emissive(0.0f);
            float metallic  = 0.0f;
            float roughness = 1.0f;

            if (mat.contains("baseColor") && mat["baseColor"].is_array() && mat["baseColor"].size() == 4) {
                baseColor = glm::vec4(
                    mat["baseColor"][0].get<float>(),
                    mat["baseColor"][1].get<float>(),
                    mat["baseColor"][2].get<float>(),
                    mat["baseColor"][3].get<float>()
                );
            }

            if (mat.contains("emissive") && mat["emissive"].is_array() && mat["emissive"].size() == 3) {
                emissive = glm::vec3(
                    mat["emissive"][0].get<float>(),
                    mat["emissive"][1].get<float>(),
                    mat["emissive"][2].get<float>()
                );
            }

            if (mat.contains("metallic"))  metallic  = mat["metallic"].get<float>();
            if (mat.contains("roughness")) roughness = mat["roughness"].get<float>();

            const auto material = CreateRef<Material>();
            material->name      = name;
            material->baseColor = baseColor;
            material->emissive  = emissive;
            material->metallic  = metallic;
            material->roughness = roughness;

            m_materials[name] = material;
        }

        return true;
    }

    Ref<Material> MaterialLibrary::GetMaterialByName(const std::string &name) {
        if (!m_materials.contains(name)) {
            Warn("Material \"" + name + "\" not found! but returning fallback\n");
            // return default material
            return GetDefaultMaterial();
        }
        return m_materials.at(name);
    }

    Ref<Material> MaterialLibrary::GetDefaultMaterial() const {
        auto textureManager = Services::GetTextureManager();

        static Ref<Material> fallback = [textureManager]{
            auto mat = CreateRef<Material>();
            mat->name      = "default";
            mat->baseColor = glm::vec4(1.0f);
            mat->emissive  = glm::vec3(0.0f);
            mat->metallic  = 0.0f;
            mat->roughness = 1.0f;

            mat->textures[MaterialTextureType::BaseColor]        = textureManager->GetDefaultTexture(MaterialTextureType::BaseColor);
            mat->textures[MaterialTextureType::Normal]           = textureManager->GetDefaultTexture(MaterialTextureType::Normal);
            mat->textures[MaterialTextureType::Roughness]        = textureManager->GetDefaultTexture(MaterialTextureType::Roughness);
            mat->textures[MaterialTextureType::Metallic]         = textureManager->GetDefaultTexture(MaterialTextureType::Metallic);
            mat->textures[MaterialTextureType::AmbientOcclusion] = textureManager->GetDefaultTexture(MaterialTextureType::AmbientOcclusion);
            mat->textures[MaterialTextureType::Emissive]         = textureManager->GetDefaultTexture(MaterialTextureType::Emissive);

            Info("Default Material added!");
            return mat;
        }();
        return fallback;
    }

    Ref<MaterialInstance> MaterialLibrary::CreateInstance(const std::string &name)
    {
        const auto base = GetMaterialByName(name);
        auto instance  = CreateRef<MaterialInstance>();
        instance->base = base;
        return instance;
    }
}



