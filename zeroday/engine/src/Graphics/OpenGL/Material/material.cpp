//
// Created by pointerlost on 9/23/25.
//
#include "Graphics/OpenGL/Material/material.h"

#include "Core/AssetManager.h"
#include "Core/Services.h"
#include "Graphics/OpenGL/GPU_buffers.h"
#include "Graphics/OpenGL/Textures/Textures.h"
#include "Scene/Components.h"

namespace Zeroday::opengl {

    MaterialSSBO MaterialInstance::ToGPUFormat() {
        auto* assetManager = Services::GetAssetManager();
        MaterialSSBO gpu{};
        gpu.baseColor         = GetBaseColor();
        gpu.emissiveMetallic  = glm::vec4(GetEmissive(), GetMetallic());
        gpu.roughnessPadding  = glm::vec4(GetRoughness(), glm::vec3(0.0f));

        auto GetHandle = [](const Ref<Texture>& tex) -> uint64_t {
            if (!tex) return 0;
            if (!tex->IsResident()) {
                tex->MakeResident();
            }
            return tex->GetBindlessHandle();
        };

        gpu.baseColorHandle    = GetHandle(GetTexture(MaterialTextureType::BaseColor, assetManager->GetDefaultTexture(MaterialTextureType::BaseColor)));
        gpu.normalHandle       = GetHandle(GetTexture(MaterialTextureType::Normal, assetManager->GetDefaultTexture(MaterialTextureType::Normal)));
        gpu.roughnessHandle    = GetHandle(GetTexture(MaterialTextureType::Roughness, assetManager->GetDefaultTexture(MaterialTextureType::Roughness)));
        gpu.displacementHandle = GetHandle(GetTexture(MaterialTextureType::Displacement, assetManager->GetDefaultTexture(MaterialTextureType::Displacement)));
        gpu.ambientOccHandle   = GetHandle(GetTexture(MaterialTextureType::AmbientOcclusion, assetManager->GetDefaultTexture(MaterialTextureType::AmbientOcclusion)));

        return gpu;
    }
}
