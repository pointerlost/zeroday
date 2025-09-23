#version 460
#include "common/constants.glsl"

// Material data
layout(std430, binding = BINDING_PHASE_OUTPUT) readonly buffer MaterialBuffer {
    MaterialSSBO materials[];
};

// Light data
layout(std430, binding = BINDING_PHASE_OUTPUT) readonly buffer LightBuffer {
    LightSSBO lights[];
};

// Global data
layout(std430, binding = BINDING_PHASE_OUTPUT) readonly buffer GlobalBuffer {
    GlobalSSBO global;
};

// Input from vertex shader
in vec3 vFragPos;
in vec3 vWorldPos;
in vec3 vNormal;
in vec2 vUV;
in vec3 vViewDir;
flat in int vMaterialIndex;

// Output
out vec4 FragColor;

void main() {
    // Fetch material
    MaterialSSBO material = materials[vMaterialIndex];

    // Basic lighting (you'll expand this with your PBR functions)
    vec3 normal = normalize(vNormal);
    vec3 viewDir = normalize(vViewDir);

    // Simple diffuse + ambient
    vec3 ambient = global.globalAmbient * material.baseColor.rgb;

    // TODO: Add pbr calculations here!!!
    vec3 lighting = ambient + material.emissive;

    FragColor = vec4(lighting, material.baseColor.a);
}