#version 460
#include "common/constants.glsl"

struct GlobalSSBO {
    vec3 globalAmbient;
    float padding1;
};

layout(std430, binding = BINDING_PHASE_INPUT) buffer CPUGlobalBuffer {
    GlobalSSBO cpuGlobalData;
};

layout(std430, binding = BINDING_PHASE_OUTPUT) buffer GPUGlobalBuffer {
    GlobalSSBO gpuGlobalData;
};

void main() {
    // Single invocation for global data
    if (gl_GlobalInvocationID.x > 0) return;

    gpuGlobalData = cpuGlobalData;
}