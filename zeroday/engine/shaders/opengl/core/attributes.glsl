#ifndef ATTRIBUTES_GLSL
#define ATTRIBUTES_GLSL

// vertex input attributes
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aUV;
layout(location = 2) in vec3 aNormal;

// vertex output attributes (those going to the fragment shader)
out vec2 vUV;
out vec3 vNormal;
out vec3 vWorldPos;

#endif