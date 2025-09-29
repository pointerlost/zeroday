#version 460
#extension GL_ARB_bindless_texture : require

#include "common/constants.glsl"
#include "Core/buffers.glsl"

// Input from vertex shader
in vec3 vFragPos;
in vec3 vNormal;
in vec2 vUV;
flat in int vMaterialIndex;

// Output
out vec4 FragColor;

uniform uint uLightCount;

vec3 CalcPBR(vec3 N, vec3 V, vec3 L, vec3 lightColor) {
    // Use vUV to sample textures
    vec3 albedo     = GetMatBaseColor(vMaterialIndex, vUV).rgb;
    float roughness = GetRoughness(vMaterialIndex, vUV);
    float metallic  = GetMetallic(vMaterialIndex);

    vec3 F0 = mix(vec3(0.04), albedo, metallic);
    vec3 H  = normalize(V + L);

    float NdotL = max(dot(N, L), 0.0);
    float NdotV = max(dot(N, V), 0.0);
    float NdotH = max(dot(N, H), 0.0);
    float VdotH = max(dot(V, H), 0.0);

    float alpha = roughness * roughness;
    float D = alpha / (PI * pow(NdotH * NdotH * (alpha - 1.0) + 1.0, 2.0) + 1e-5);
    float G = min(1.0, min(2.0 * NdotH * NdotV / max(VdotH, 1e-5), 2.0 * NdotH * NdotL / max(VdotH, 1e-5)));
    vec3 F = F0 + (1.0 - F0) * pow(1.0 - VdotH, 5.0);

    vec3 specular = (D * G * F) / (4.0 * max(NdotL * NdotV, 1e-5) + 1e-5);
    vec3 kD = (1.0 - F) * (1.0 - metallic);

    vec3 Lo = (kD * albedo / PI + specular) * lightColor * NdotL;
    return Lo;
}

vec3 CalcDirectionalLight(LightSSBO light, vec3 worldPos, vec3 worldNormal) {
    vec3 L = normalize(-light.direction);
    vec3 V = normalize(GetCameraPos() - worldPos);
    vec3 N = normalize(worldNormal);

    return CalcPBR(N, V, L, light.radiance * light.intensity);
}

vec3 CalcPointLight(LightSSBO light, vec3 worldPos, vec3 worldNormal) {
    vec3 L = normalize(light.position - worldPos);
    vec3 V = normalize(GetCameraPos() - worldPos);
    vec3 N = normalize(worldNormal);

    float distance = length(light.position - worldPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    return CalcPBR(N, V, L, light.radiance * light.intensity) * attenuation;
}

vec3 CalcSpotLight(LightSSBO light, vec3 worldPos, vec3 worldNormal) {
    vec3 L = normalize(light.position - worldPos);
    vec3 V = normalize(GetCameraPos() - worldPos);
    vec3 N = normalize(worldNormal);

    float theta     = dot(L, normalize(-light.direction));
    float epsilon   = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    float distance    = length(light.position - worldPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    return CalcPBR(N, V, L, light.radiance * light.intensity) * attenuation * intensity;
}

void main() {
    vec3 result = vec3(0.0);

    vec3 worldPos    = vFragPos;
    vec3 worldNormal = normalize(vNormal);

    // Process lights
    for (int i = 0; i < uLightCount; i++) {
        LightSSBO light = lights[i];

        if (light.type == 0) { // Directional
            result += CalcDirectionalLight(light, worldPos, worldNormal);
        } else if (light.type == 1) { // Point
            result += CalcPointLight(light, worldPos, worldNormal);
        } else if (light.type == 2) { // Spot
            result += CalcSpotLight(light, worldPos, worldNormal);
        }
    }

    // Use vUV for ambient sampling too
    vec3 ambientColor = GetMatBaseColor(vMaterialIndex, vUV).rgb;
    result += ambientColor * uGlobal.globalAmbient;

    // Add emissive
    result += GetEmissive(vMaterialIndex);

    // Gamma correction
    result = pow(result, vec3(1.0/2.2));

    FragColor = vec4(result, 1.0);
}
