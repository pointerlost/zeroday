#version 460
layout(local_size_x = 64) in;

#include "common/constants.glsl"

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

layout(std430, binding = BINDING_PHASE_INPUT) buffer CPULightBuffer {
    LightSSBO cpuLights[];
};

layout(std430, binding = BINDING_PHASE_OUTPUT) buffer GPULightBuffer {
    LightSSBO gpuLights[];
};

void main() {
    uint lightId = gl_GlobalInvocationID.x;
    if (lightId >= MAX_ENTITIES) return;

    gpuLights[lightId] = cpuLights[lightId];
}