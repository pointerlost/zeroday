#ifndef CAMERA_DATA_GLSL
#define CAMERA_DATA_GLSL

#include "common/constants.glsl"

struct CameraData {
    mat4 viewProjection;
    mat4 view;
    mat4 projection;
    vec3 position;
    float padding1;
    vec3 direction;
    float padding2;
};

layout(std430, binding = BINDING_PHASE_METADATA) uniform CameraUBO {
    CameraData cameras[MAX_CAMERAS];  // Array of cameras
    int activeCameraCount;            // How many cameras are valid
    int mainCameraIndex;              // Which camera to use for main rendering
} uCameras;

// Helper function to get current camera
CameraData GetCamera(int index) {
    return uCameras.cameras[index];
}

mat4 GetViewMatrix(int cameraIndex)       { return uCameras.cameras[cameraIndex].view; }
mat4 GetProjectionMatrix(int cameraIndex) { return uCameras.cameras[cameraIndex].projection; }
vec3 GetCameraPosition(int cameraIndex)   { return uCameras.cameras[cameraIndex].position; }

// Default to main camera
mat4 GetViewMatrix()       { return GetViewMatrix(uCameras.mainCameraIndex); }
mat4 GetProjectionMatrix() { return GetProjectionMatrix(uCameras.mainCameraIndex); }

vec4 WorldToClipSpace(vec3 worldPosition, int cameraIndex) {
    return uCameras.cameras[cameraIndex].viewProjection * vec4(worldPosition, 1.0);
}

vec4 WorldToClipSpace(vec3 worldPosition) {
    return WorldToClipSpace(worldPosition, uCameras.mainCameraIndex);
}

#endif