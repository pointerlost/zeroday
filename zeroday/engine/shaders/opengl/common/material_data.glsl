#version 460
layout(local_size_x = 64) in;

#include "common/constants.glsl"

struct MaterialSSBO {
    vec4 baseColor;
    vec3 emissive;
    float padding1;
    float metallic;
    float roughness;
    float padding2;
    float padding3;
};

// CPU -> GPU data flow
layout(std430, binding = BINDING_PHASE_INPUT) buffer CPUMaterialBuffer {
    MaterialSSBO cpuMaterials[];
};

// GPU-side storage
layout(std430, binding = BINDING_PHASE_OUTPUT) buffer GPUMaterialBuffer {
    MaterialSSBO gpuMaterials[];
};

void main() {
    uint entityId = gl_GlobalInvocationID.x;
    if (entityId >= MAX_ENTITIES) return;

    gpuMaterials[entityId] = cpuMaterials[entityId];
}