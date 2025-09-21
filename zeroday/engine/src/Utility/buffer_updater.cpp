//
// Created by pointerlost on 9/1/25.
//
#include "Utility/buffer_updater.h"
#include "Graphics/OpenGL/Lighting/Light.h"
#include "Graphics/OpenGL/Material/material.h"
#include "Graphics/OpenGL/Material/material_lib.h"
#include "Graphics/OpenGL/Renderer/RenderContext.h"
#include "Graphics/OpenGL/Textures/TextureManager.h"
#include "Graphics/OpenGL/Transformations/Transformations.h"

namespace opengl {

    Zeroday::MaterialSSBO BufferUpdater::MakeGPUMaterialInstance(const std::shared_ptr<Zeroday::MaterialInstance> &inst) {
        Zeroday::MaterialSSBO gpu{};
        gpu.baseColor = inst->getBaseColor();
        gpu.metallic  = inst->getMetallic();
        gpu.roughness = inst->getRoughness();
        gpu.emissive  = inst->getEmissive();

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

    std::vector<ContiguousRange> BufferUpdater::FindContiguousRanges(const std::vector<uint32_t> &indices) {
        std::vector<ContiguousRange> ranges;
        if (indices.empty()) return ranges;

        std::vector<uint32_t> sorted = indices;
        std::ranges::sort(sorted);

        uint32_t start = sorted[0];
        uint32_t count = 1;

        for (size_t i = 1; i < sorted.size(); ++i) {
            if (sorted[i] == sorted[i - 1] + 1) {
                count++;
            } else {
                ranges.push_back({start, count});
                start = sorted[i];
                count = 1;
            }
        }
        ranges.push_back({start, count});

        return ranges;
    }

    void BufferUpdater::UpdateTransformComponent(const ecs::TransformComponent *comp, Zeroday::TransformSSBO &gpuData) {
        const auto modelMat = comp->transform->getModelMatrix();
        const auto normalMat = glm::transpose(glm::inverse(glm::mat3(modelMat)));
        gpuData.model = modelMat;
        gpuData.normalMat = normalMat;
    }

    void BufferUpdater::UpdateMaterialComponent(const ecs::MaterialComponent *comp, Zeroday::MaterialSSBO &gpuData) {
        gpuData = MakeGPUMaterialInstance(comp->instance);
    }

    void BufferUpdater::UpdateLightComponent(const ecs::LightComponent *comp, Zeroday::LightSSBO &gpuData) {
        auto& light = comp->light;
        gpuData.type = static_cast<int>(light->getType());

        switch (light->getType()) {
            case LIGHTING::LightType::Directional: {
                gpuData.direction = light->getDirection();
            } break;

            case LIGHTING::LightType::Point: {
                gpuData.position  = light->getPosition();
                gpuData.constant  = light->getConstant();
                gpuData.linear    = light->getLinear();
                gpuData.quadratic = light->getQuadratic();
            } break;

            case LIGHTING::LightType::Spot: {
                gpuData.position    = light->getPosition();
                gpuData.direction   = light->getDirection();
                gpuData.cutOff      = light->getCutOff();
                gpuData.outerCutOff = light->getOuterCutOff();
                gpuData.constant    = light->getConstant();
                gpuData.linear      = light->getLinear();
                gpuData.quadratic   = light->getQuadratic();
            } break;
        }
    }

    void BufferUpdater::UpdateCameraComponent(const ecs::CameraComponent *comp, Zeroday::CameraSSBO &gpuData) {
        gpuData.view       = comp->GetViewMatrix();
        gpuData.projection = comp->GetProjectionMatrix();
    }
}
