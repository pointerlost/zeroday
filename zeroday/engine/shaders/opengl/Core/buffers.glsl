#ifndef BUFFERS_GLSL
#define BUFFERS_GLSL

#extension GL_ARB_bindless_texture : require
#extension GL_ARB_gpu_shader_int64 : require

// ************************** TRANSFORM BUFFER *********************

struct TransformSSBO {
    mat4 modelMatrix;
    mat4 normalMatrix;
};

layout(std430, binding = 3) buffer TransformBuffer {
    TransformSSBO transforms[];
};

// ************************** MATERIAL BUFFER *********************
struct MaterialSSBO {
    // material
    vec4 baseColor;
    vec4 emissiveMetallic;  // xyz: emissive, w: metallic - 16 bytes
    vec4 roughnessPadding;  // x: roughness, yzw: padding - 16 bytes

    // texture handles
    uint64_t baseColorHandle;
    uint64_t normalHandle;
    uint64_t roughnessHandle;
    uint64_t displacementHandle;
    uint64_t ambientOccHandle;
    uint64_t padding[1];
};

layout(std430, binding = 4) buffer MaterialBuffer {
    MaterialSSBO materials[];
};

// helpers
vec4 GetMatBaseColor(int materialIndex, vec2 uv) {
    if (materials[materialIndex].baseColorHandle != 0u) {
        return texture(sampler2D(materials[materialIndex].baseColorHandle), uv);
    } else {
        return materials[materialIndex].baseColor;
    }
}

vec3 GetNormal(int materialIndex, vec2 uv) {
    if (materials[materialIndex].normalHandle != 0u) {
        return texture(sampler2D(materials[materialIndex].normalHandle), uv).xyz * 2.0 - 1.0;
    } else {
        return vec3(0.0, 0.0, 1.0); // default normal
    }
}

float GetRoughness(int materialIndex, vec2 uv) {
    if (materials[materialIndex].roughnessHandle != 0u) {
        return texture(sampler2D(materials[materialIndex].roughnessHandle), uv).r;
    } else {
        return materials[materialIndex].roughnessPadding.x;
    }
}

vec3  GetEmissive(int materialIndex) { return materials[materialIndex].emissiveMetallic.xyz; }
float GetMetallic(int materialIndex) { return materials[materialIndex].emissiveMetallic.w; }

// ************************** LIGHT BUFFER *********************

struct LightSSBO {
    int   type;
    float constant;
    float linear;
    float quadratic;
    vec3  position;
    float cutOff;
    vec3  direction;
    float outerCutOff;
    vec3  radiance;
    float intensity;
};

layout(std430, binding = 5) buffer LightBuffer {
    LightSSBO lights[];
};

// ************************** CAMERA UBO *********************

layout(std140, binding = 6) uniform CameraUBO {
    mat4 viewProjection;
    mat4 view;
    mat4 projection;
    vec4 position;  // w unused (padding)
    vec4 direction; // w unused
} uCamera;

// helpers
mat4 GetProjView()   { return uCamera.viewProjection; }
mat4 GetView()       { return uCamera.view;           }
mat4 GetProjection() { return uCamera.projection;     }
vec3 GetCameraPos()  { return uCamera.position.xyz;   }
vec3 GetCameraDir()  { return uCamera.direction.xyz;  }

vec4 WorldToClipSpace(vec3 worldPosition) {
    return uCamera.viewProjection * vec4(worldPosition, 1.0);
}

// ************************** GLOBAL UBO *********************

layout(std140, binding = 7) uniform GlobalUBO {
    vec3 globalAmbient;
    float padding1;
} uGlobal;

// helpers
vec3 GetGlobalAmbient() { return uGlobal.globalAmbient; }

// ************************** GLOBAL UBO *********************

#endif