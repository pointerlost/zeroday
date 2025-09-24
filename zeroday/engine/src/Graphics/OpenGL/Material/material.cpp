//
// Created by pointerlost on 9/23/25.
//
#include "Graphics/OpenGL/Material/material.h"
#include "Graphics/OpenGL/GPU_buffers.h"
#include "Scene/Components.h"

namespace Zeroday::opengl {

    MaterialSSBO MaterialInstance::ToGPUFormat() {
        MaterialSSBO gpu{};
        gpu.baseColor = GetBaseColor();
        gpu.metallic  = GetMetallic();
        gpu.roughness = GetRoughness();
        gpu.emissive  = GetEmissive();

        // auto* texManager = Services::GetTextureManager();
        // auto getHandle = [&](Graphics::MaterialTextureType type) -> uint64_t {
        //     if (const auto tex = inst->getTexture(type))
        //         return texManager->ensureBindlessHandle(tex);
        //     return 0;
        // };
        //
        // gpu.baseColorHandle    = getHandle(Graphics::MaterialTextureType::BaseColor);
        // gpu.normalHandle       = getHandle(Graphics::MaterialTextureType::Normal);
        // gpu.metallicHandle     = getHandle(Graphics::MaterialTextureType::Metallic);
        // gpu.roughnessHandle    = getHandle(Graphics::MaterialTextureType::Roughness);
        // gpu.emissiveHandle     = getHandle(Graphics::MaterialTextureType::Emissive);
        // gpu.aoHandle           = getHandle(Graphics::MaterialTextureType::AmbientOcclusion);
        // gpu.displacementHandle = getHandle(Graphics::MaterialTextureType::Displacement);
        // gpu.heightHandle       = getHandle(Graphics::MaterialTextureType::Height);

        return gpu;
    }
}
