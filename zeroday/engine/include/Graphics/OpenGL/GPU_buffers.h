//
// Created by pointerlost on 8/19/25.
//
#pragma once
#include <glm/ext.hpp>
#include "Macros.h"

namespace LIGHTING { enum class LightType; }

namespace Zeroday::opengl {

    struct TransformSSBO {
        glm::mat4 modelMatrix  = glm::mat4(1.0f); // world matrix
        glm::mat4 normalMatrix = glm::mat4(1.0f);
    };
    static_assert(sizeof(TransformSSBO) % 16 == 0);
    static_assert(std::is_trivially_copyable_v<TransformSSBO>);
    /********************************************************************************/

    struct GlobalSSBO {
        glm::vec3 globalAmbient = glm::vec3(0.1f);
        float padding1 = 0.0f;
    };
    static_assert(sizeof(GlobalSSBO) % 16 == 0);
    /********************************************************************************/

    struct LightSSBO {
        int type = 1; // 0 = directional, 1 = point, 2 = spot
        float constant       = 1.0f;
        float linear         = 0.5f;
        float quadratic      = 0.772f;
        glm::vec3 position   = glm::vec3(0.0f); // world space
        float cutOff         = 0.0f;
        glm::vec3 direction  = glm::vec3(0.0f);
        float outerCutOff    = 0.0f;
        glm::vec3 radiance   = glm::vec3(0.0f); // light color * intensity
        float intensity      = 32.0f; // scalar multiplier (optional, can merge with radiance)
    };
    static_assert(sizeof(LightSSBO) % 16 == 0); // helps detect ordering/padding mistakes
    /********************************************************************************/

    struct MaterialSSBO {
        glm::vec4 baseColor = glm::vec4(1.0f);  // (RGBA)
        glm::vec3 emissive  = glm::vec3(0.1f);
        float     padding1  = 0.0f;  // 16 byte alignment
        float     metallic  = 0.3f;
        float     roughness = 0.2f;
        float     padding2  = 0.0f;
        float     padding3  = 0.0f;

        // uint64_t baseColorHandle;  // bindless texture handle
        // uint64_t normalHandle;
        // uint64_t roughnessHandle;
        // uint64_t metallicHandle;
        // uint64_t aoHandle;
        // uint64_t emissiveHandle;
        // uint64_t displacementHandle;
        // uint64_t heightHandle;
    };
    static_assert(sizeof(MaterialSSBO) % 16 == 0);

    struct CameraUBO {
        glm::mat4 viewProjection = glm::mat4(1.0f);
        glm::mat4 view           = glm::mat4(1.0f);
        glm::mat4 projection     = glm::mat4(1.0f);
        glm::vec3 position       = glm::vec3(0.0f);
        float padding1           = 0.0f;
        glm::vec3 direction      = glm::vec3(0.0f);
        float padding2           = 0.0f;
    };
    static_assert(sizeof(CameraUBO) % 16 == 0);

}