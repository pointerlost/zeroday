#ifndef BUFFERS_GLSL
#define BUFFERS_GLSL

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
    vec4 baseColor; // 16 bytes
    vec4 emissiveMetallic;  // xyz: emissive, w: metallic - 16 bytes
    vec4 roughnessPadding;  // x: roughness, yzw: padding - 16 bytes
};

layout(std430, binding = 4) buffer MaterialBuffer {
    MaterialSSBO materials[];
};

// helpers
vec4 GetMatBaseColor(int idx) { return materials[idx].baseColor; }
vec3 GetEmissive(int idx) { return materials[idx].emissiveMetallic.xyz; }
float GetMetallic(int idx) { return materials[idx].emissiveMetallic.w; }
float GetRoughness(int idx) { return materials[idx].roughnessPadding.x; }

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