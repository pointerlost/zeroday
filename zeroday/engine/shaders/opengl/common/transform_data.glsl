#version 460
layout(local_size_x = 64) in;

#include "common/constants.glsl"

struct TransformSSBO {
    mat4 worldMatrix;
    mat4 normalMatrix;
};

// CPU -> GPU data flow (upload from CPU)
layout(std430, binding = BINDING_PHASE_INPUT) buffer CPUTransformBuffer {
    mat4 cpuTransforms[];
};

// GPU-side storage (used by rendering)
layout(std430, binding = BINDING_PHASE_OUTPUT) buffer GPUTransformBuffer {
    TransformSSBO gpuTransforms[];
};

void main() {
    uint entityId = gl_GlobalInvocationID.x;
    if (entityId >= MAX_ENTITIES) return;

    mat4 worldMatrix = cpuTransforms[entityId];
    mat3 normalMatrix = mat3(transpose(inverse(mat3(worldMatrix))));

    gpuTransforms[entityId].worldMatrix = worldMatrix;
    gpuTransforms[entityId].normalMatrix = mat4(normalMatrix);
}