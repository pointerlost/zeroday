//
// Created by pointerlost on 9/23/25.
//
#include "Graphics/OpenGL/Material/material.h"
#include "Core/Services.h"
#include "Graphics/OpenGL/GPU_buffers.h"
#include "Graphics/OpenGL/Textures/Textures.h"
#include "Scene/Components.h"

namespace Zeroday::opengl {

    MaterialSSBO MaterialInstance::ToGPUFormat() {
        MaterialSSBO gpu{};
        gpu.baseColor         = GetBaseColor();
        gpu.emissiveMetallic  = glm::vec4(GetEmissive(), GetMetallic());
        gpu.roughnessPadding  = glm::vec4(GetRoughness(), glm::vec3(0.0f));

        auto getHandle = [](const Ref<Texture>& tex) -> uint64_t {
            return tex ? tex->MakeResident() : 0; // Ensure resident and get handle
        };

        gpu.baseColorHandle    = getHandle(GetTexture(MaterialTextureType::BaseColor));
        gpu.normalHandle       = getHandle(GetTexture(MaterialTextureType::Normal));
        gpu.roughnessHandle    = getHandle(GetTexture(MaterialTextureType::Roughness));
        gpu.displacementHandle = getHandle(GetTexture(MaterialTextureType::Displacement));
        gpu.ambientOccHandle   = getHandle(GetTexture(MaterialTextureType::AmbientOcclusion));

        return gpu;
    }
}
