#version 460 core

#include "common/constant.glsl"
#include "compute/pbr_calc.comp"
#include "compute/lighting_calc.comp"


// -------------------
// Inputs / Outputs
// -------------------
in vec3 FragPos;
in vec4 vWorldPos;
in vec3 vNormal;
in vec2 vUV;
in vec3 vViewDir;


// -------------------
// Main
// -------------------
void main() {

    vec3 N = vNormal;
    vec3 V = vViewDir;

    vec3 result = m.emissive + globals[0].globalAmbient;

    for (int i = 0; i < globals[0].activeLightCount; ++i) {
        if (lights[i].type == 0)
            result += CalcDirectionalLight(lights[i], N, V, m);
        else if (lights[i].type == 1)
            result += CalcPointLight(lights[i], FragPos, N, V, m);
        else if (lights[i].type == 2)
            result += CalcSpotLight(lights[i], FragPos, N, V, m);
    }

    // simple tonemapping/clamp if desired (optional)
    FragColor = vec4(result, 1.0);
}
