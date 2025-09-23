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
