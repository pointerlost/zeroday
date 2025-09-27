//
// Created by pointerlost on 9/23/25.
//
#include "Graphics/OpenGL/Material/material.h"

#include "core/Services.h"
#include "Graphics/OpenGL/GPU_buffers.h"
#include "Graphics/OpenGL/Textures/TextureManager.h"
#include "Scene/Components.h"

namespace Zeroday::opengl {

    MaterialSSBO MaterialInstance::ToGPUFormat() {
        MaterialSSBO gpu{};
        auto emissiveMetallic = glm::vec4(GetEmissive(), GetMetallic());
        gpu.baseColor        = GetBaseColor();
        gpu.emissiveMetallic = emissiveMetallic;
        gpu.roughnessPadding = glm::vec4(GetRoughness(), glm::vec3(0.0f));

        // auto* texManager = Services::GetTextureManager();
        // auto getHandle = [&](MaterialTextureType type) -> uint64_t {
        //     if (const auto tex = GetTexture(type))
        //         return texManager->EnsureBindlessHandle(tex);
        //     return 0;
        // };
        //
        // gpu.baseColor    = getHandle(MaterialTextureType::BaseColor);
        // gpu.       = getHandle(MaterialTextureType::Normal);
        // gpu.metallicHandle     = getHandle(MaterialTextureType::Metallic);
        // gpu.roughnessHandle    = getHandle(MaterialTextureType::Roughness);
        // gpu.emissiveHandle     = getHandle(MaterialTextureType::Emissive);
        // gpu.aoHandle           = getHandle(MaterialTextureType::AmbientOcclusion);
        // gpu.displacementHandle = getHandle(MaterialTextureType::Displacement);
        // gpu.heightHandle       = getHandle(MaterialTextureType::Height);

        return gpu;
    }
}
