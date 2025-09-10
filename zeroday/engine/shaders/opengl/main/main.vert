#version 460 core

#include "core/attributes.glsl"

out vec3 FragPos;
out vec4 vWorldPos;
out vec3 vNormal;
out vec2 vUV;
out vec3 vViewDir;

void main() {
    // items to be sent to the fragment shader
    vUV       = aUV;
    vNormal   = mat3(normalMatrix) * aNormal;
    vWorldPos = modelMatrix * vec4(aPosition, 1.0f);
    FragPos   = vWorldPos.xyz;
    // vViewDir calculation...
    // -------------------------------------------- //

    // need view, projection!! and use ApplyViewProjection!!!

    gl_Position = projection * view * worldPos;
}