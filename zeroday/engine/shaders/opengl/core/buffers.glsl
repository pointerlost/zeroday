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
    vec4 baseColor;
    vec3 emissive;
    float padding1;
    float metallic;
    float roughness;
    float padding2;
    float padding3;
};

layout(std430, binding = 4) buffer MaterialBuffer {
    MaterialSSBO materials[];
};

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
    vec3 position;
    float padding1;
    vec3 direction;
    float padding2;
} uCamera;

// helpers
mat4 GetViewProj()   { return uCamera.viewProjection; }
mat4 GetView()       { return uCamera.view;           }
mat4 GetProjection() { return uCamera.projection;     }
vec3 GetCameraPos()  { return uCamera.position;       }
vec3 GetCameraDir()  { return uCamera.direction;      }

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