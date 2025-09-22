#version 460
#include "common/constants.glsl"
#include "common/attributes.glsl"

// Draw payload (from compute shader)
layout(std430, binding = BINDING_PAYLOADS) readonly buffer DrawPayloads {
    DrawPayloadGPU payloads[];
};

// Transform data (updated by compute shader)
layout(std430, binding = BINDING_PHASE_OUTPUT) readonly buffer TransformBuffer {
    TransformSSBO transforms[];
};

// Camera data
#include "common/camera_data.glsl"

// Output to fragment shader
out vec3 vFragPos;
out vec3 vWorldPos;
out vec3 vNormal;
out vec2 vUV;
out vec3 vViewDir;
flat out int vMaterialIndex;

void main() {
    // Get payload for this draw instance
    int payloadIndex = gl_BaseInstance;
    DrawPayloadGPU payload = payloads[payloadIndex];

    // Fetch transform data
    TransformSSBO transform = transforms[payload.transformIndex];

    // Transform to world space
    vec4 worldPos = transform.worldMatrix * vec4(aPosition, 1.0);
    vWorldPos = worldPos.xyz;
    vFragPos = worldPos.xyz;

    // Transform normal
    vNormal = mat3(transform.normalMatrix) * aNormal;

    // Pass through UV
    vUV = aUV;

    // Calculate view direction (world space)
    vViewDir = normalize(uCamera.cameraPosition - vWorldPos);

    // Set material index for fragment shader
    vMaterialIndex = payload.materialIndex;

    // Final position - use efficient viewProjection matrix
    gl_Position = uCamera.viewProjection * worldPos;
}