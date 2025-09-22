#ifndef CAMERA_DATA_GLSL
#define CAMERA_DATA_GLSL

#include "common/constants.glsl"

layout(std430, binding = BINDING_PHASE_METADATA) uniform CameraUBO {
    mat4 viewProjection;        // Combined matrix (performance)
    mat4 viewMatrix;           // Separate for lighting
    mat4 projectionMatrix;     // Separate for reconstruction
    vec3 cameraPosition;
    float padding1;
    vec3 cameraDirection;
    float padding2;
} uCamera;

mat4 GetViewMatrix()       { return uCamera.viewMatrix; }
mat4 GetProjectionMatrix() { return uCamera.projectionMatrix; }
vec3 GetCameraPosition()   { return uCamera.cameraPosition; }
vec3 GetCameraDirection()  { return uCamera.cameraDirection; }

vec4 WorldToClipSpace(vec3 worldPosition) {
    return uCamera.viewProjection * vec4(worldPosition, 1.0);
}

#endif