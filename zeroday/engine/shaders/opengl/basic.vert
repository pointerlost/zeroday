#version 460 core
#extension GL_ARB_bindless_texture : require
#extension GL_ARB_gpu_shader_int64 : enable

layout(std140, binding = 0) uniform GlobalOffsets {
    int transformOffset;
    int materialOffset;
    int lightOffset;
    int cameraOffset;
    int globalOffset;
} offsets;

// --- MDI Payload SSBO ---
struct DrawPayload {
    int transformIndex;
    int materialIndex;
    int meshID;
    int pad;
};

layout(std430, binding = 1) buffer DrawPayloadBuffer {
    DrawPayload payloads[];
};

// --- Transform SSBO ---
struct TransformData {
    mat4 model;
    mat4 normalMat;
};

layout(std430, binding = 2) buffer TransformBuffer {
    TransformData transforms[];
};

// --- Camera SSBO ---
layout(std430, binding = 3) buffer CameraBuffer {
    mat4 view;
    mat4 projection;
};

// --- Inputs ---
layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inUV;

// --- Outputs ---
out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;
flat out int vMaterialIndex;

void main() {
    uint drawID = gl_BaseInstance;

        // get original index from payload
        int originalTransformIndex = payloads[drawID].transformIndex;
        int originalMaterialIndex = payloads[drawID].materialIndex;

        // add offsets to index for per-ssbo type
        int offsetTransformIndex = originalTransformIndex + offsets.transformOffset;
        int offsetMaterialIndex = originalMaterialIndex + offsets.materialOffset;

        vMaterialIndex = offsetMaterialIndex; // send to fragment shader

        mat4 modelMatrix = transforms[offsetTransformIndex].model;
        mat4 normalMatrix = transforms[offsetTransformIndex].normalMat;

        vec4 worldPos = modelMatrix * vec4(inPos, 1.0);
        FragPos = worldPos.xyz;
        Normal = mat3(normalMatrix) * inNormal;
        TexCoords = inUV;

        gl_Position = projection * view * worldPos;
}