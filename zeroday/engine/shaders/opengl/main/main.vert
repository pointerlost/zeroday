#version 460

#include "core/buffers.glsl"
#include "common/constants.glsl"
#include "common/command_generation.glsl"

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUV;

// Output to fragment shader
out vec3 vFragPos; // for lighting
out vec2 vUV;
out vec3 vNormal;
flat out int vMaterialIndex;

void main() {
    // Get payload for this draw instance
    int payloadIndex = gl_BaseInstance;
    DrawPayloadGPU payload = payloads[payloadIndex];

    // Fetch transform data
    TransformSSBO transform = transforms[payload.transformIndex];

    // Transform to world space
    vec4 worldPos = transform.modelMatrix * vec4(aPosition, 1.0);
    vFragPos  = worldPos.xyz;

    // Transform normal
    vNormal = mat3(transform.normalMatrix) * aNormal;
    vUV = aUV;
    vMaterialIndex = payload.materialIndex;

    // Final position
    gl_Position = uCamera.viewProjection * worldPos;
}