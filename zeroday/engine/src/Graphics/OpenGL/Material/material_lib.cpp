#include "Graphics/OpenGL/Material/material_lib.h"
#include "core/File.h"
#include "Graphics/OpenGL/Textures/TextureManager.h"
#include "core/Logger.h"
#include "core/Services.h"
#include "Graphics/OpenGL/ssbo_types.h"
#define DEBUG_PTR(ptr) DEBUG::DebugForEngineObjectPointers(ptr)

static auto& file = Zeroday::File::get();

namespace Zeroday
{
    bool MaterialLibrary::createMaterials(const std::string& filePath)
    {
        std::string content = file.readFromFile(filePath);
        if (content.empty()) {
            Logger::Warn("[MaterialLibrary::createMaterials] content is empty!\n");
            return false;
        }

        json parsed = json::parse(content);
        if (!parsed.contains("materials") || !parsed["materials"].is_array()) {
            Logger::Warn("[MaterialLibrary::createMaterials] Invalid material file structure!\n");
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

            const auto material = std::make_shared<Material>();
            material->name      = name;
            material->baseColor = baseColor;
            material->emissive  = emissive;
            material->metallic  = metallic;
            material->roughness = roughness;

            m_materials[name] = material;
        }

        return true;
    }

    std::shared_ptr<Material> MaterialLibrary::getMaterialByName(const std::string &name) {
        if (!m_materials.contains(name)) {
            Logger::Warn("Material \"" + name + "\" not found! but returning fallback\n");
            // return default material
            return getDefaultMaterial();
        }
        return m_materials.at(name);
    }

    std::shared_ptr<Material> MaterialLibrary::getDefaultMaterial() const {
        auto textureManager = Services::GetTextureManager();

        static std::shared_ptr<Material> fallback = [textureManager]{
            auto mat = std::make_shared<Material>();
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

            Logger::Info("Default Material added!");
            return mat;
        }();
        return fallback;
    }

    std::shared_ptr<MaterialInstance> MaterialLibrary::createInstance(const std::string &name)
    {
        const auto base = getMaterialByName(name);
        auto instance = std::make_shared<MaterialInstance>();
        instance->base = base;
        return instance;
    }
}



