//
// Created by pointerlost on 8/19/25.
//
#pragma once
#include <glm/ext.hpp>

// SSBO binding locations
constexpr int PAYLOAD_SSBO_IDX     = 1;
constexpr int TRANSFORM_SSBO_IDX   = 2;
constexpr int CAMERA_SSBO_IDX      = 3;
constexpr int MATERIAL_SSBO_IDX    = 4;
constexpr int LIGHT_SSBO_IDX       = 5;
constexpr int GLOBAL_DATA_SSBO_IDX = 6;

namespace LIGHTING { enum class LightType; }

namespace Zeroday {

    struct alignas(16) TransformSSBO {
        glm::mat4 model     = glm::mat4(1.0f);
        glm::mat4 normalMat = glm::mat4(1.0f);
    };
    static_assert(sizeof(TransformSSBO) % 16 == 0);
    static_assert(std::is_trivially_copyable_v<TransformSSBO>);
    /********************************************************************************/

    struct alignas(16) CameraSSBO {
        glm::mat4 view       = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
    };
    static_assert(sizeof(CameraSSBO) % 16 == 0);
    /********************************************************************************/

    struct alignas(16) GlobalSSBO {
        glm::vec3 globalAmbient = glm::vec3(0.1f);
        float _p1    = {};

        glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, -5.0f);
        float _p2    = {};

        int activeLightCount = 0;
        float _p3[3] = {};
    };
    static_assert(sizeof(GlobalSSBO) % 16 == 0);
    /********************************************************************************/

    struct alignas(16) LightSSBO {
        int   type = 1;          // 0 = directional, 1 = point, 2 = spot
        float constant       = {};
        float linear         = {};
        float quadratic      = {};
        glm::vec3  position  = {}; // world space
        float cutOff         = {};
        glm::vec3  direction = {};
        float outerCutOff    = {};
        glm::vec3  radiance  = {}; // light color * intensity
        float intensity      = {};     // scalar multiplier (optional, can merge with radiance)
    };
    static_assert(sizeof(LightSSBO) % 16 == 0); // helps detect ordering/padding mistakes
    /********************************************************************************/

    struct MaterialSSBO {
        glm::vec4 baseColor = {};  // (RGBA)
        glm::vec3 emissive  = {};
        float     _p1       = {};       // 16 byte alignment
        float     metallic  = {};
        float     roughness = {};
        float     _p2       = {};
        float     _p3       = {};

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

}