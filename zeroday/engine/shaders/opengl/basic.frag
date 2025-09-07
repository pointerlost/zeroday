#version 460 core
#define PI 3.14159265359

// -------------------
// Material SSBO (no texture handles)
// -------------------
struct MaterialData {
    vec4 baseColor;   // rgb = albedo, a = unused (or alpha)
    vec3 emissive;
    float _p1;
    float metallic;
    float roughness;
    float _p2;
    float _p3;
};

layout(std430, binding = 4) buffer MaterialBuffer {
    MaterialData materials[];
};

// -------------------
// Light SSBO
// -------------------
struct LightData {
    int   type; // 0 = directional, 1 = point, 2 = spot
    float constant;
    float linear;
    float quadratic;
    vec3  position;
    float cutOff;
    vec3  direction;
    float outerCutOff;
    vec3  radiance; // color * intensity
    float intensity;
};

layout(std430, binding = 5) buffer LightBuffer {
    LightData lights[];
};

// -------------------
// Global data SSBO
// -------------------
struct GlobalData {
    vec3 globalAmbient;
    float _p1;
    vec3 cameraPos;
    float _p2;
    int activeLightCount;
    float _p3[3];
};

layout(std430, binding = 6) buffer GlobalBuffer {
    GlobalData globals[];
};

// -------------------
// Inputs / Outputs
// -------------------
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
flat in int vMaterialIndex;

out vec4 FragColor;

// -------------------
// PBR calculation (uses material fields directly)
// -------------------
vec3 CalcPBR(MaterialData m, vec3 N, vec3 V, vec3 L, vec3 lightColor) {
    vec3 albedo = m.baseColor.rgb;
    float rough = m.roughness;
    float metal = m.metallic;

    vec3 F0 = mix(vec3(0.04), albedo, metal);
    vec3 H = normalize(V + L);

    float NdotL = max(dot(N, L), 0.0);
    float NdotV = max(dot(N, V), 0.0);
    float NdotH = max(dot(N, H), 0.0);
    float VdotH = max(dot(V, H), 0.0);

    float alpha = rough * rough;
    float D = alpha / (PI * pow(NdotH * NdotH * (alpha - 1.0) + 1.0, 2.0) + 1e-5);
    float G = min(1.0, min(2.0 * NdotH * NdotV / max(VdotH, 1e-5), 2.0 * NdotH * NdotL / max(VdotH, 1e-5)));
    vec3 F = F0 + (1.0 - F0) * pow(1.0 - VdotH, 5.0);

    vec3 specular = (D * G * F) / (4.0 * max(NdotL * NdotV, 1e-5) + 1e-5);
    vec3 kD = (1.0 - F) * (1.0 - metal);

    vec3 Lo = (kD * albedo / PI + specular) * lightColor * NdotL;
    return Lo;
}

// -------------------
// Light models (use MaterialData)
vec3 CalcDirectionalLight(LightData light, vec3 N, vec3 V, MaterialData m) {
    vec3 L = normalize(-light.direction);
    return CalcPBR(m, N, V, L, light.radiance * light.intensity);
}

vec3 CalcPointLight(LightData light, vec3 fragPos, vec3 N, vec3 V, MaterialData m) {
    vec3 L = normalize(light.position - fragPos);
    float d = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * d + light.quadratic * d * d);
    return CalcPBR(m, N, V, L, light.radiance * light.intensity) * attenuation;
}

vec3 CalcSpotLight(LightData light, vec3 fragPos, vec3 N, vec3 V, MaterialData m) {
    vec3 L = normalize(light.position - fragPos);
    float theta = dot(L, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float spotFactor = clamp((theta - light.outerCutOff) / max(epsilon, 1e-5), 0.0, 1.0);

    float d = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * d + light.quadratic * d * d);

    return CalcPBR(m, N, V, L, light.radiance * light.intensity) * attenuation * spotFactor;
}

// -------------------
// Main
// -------------------
void main() {
    int mi = vMaterialIndex;
    if (mi < 0) {
        FragColor = vec4(1.0, 0.0, 1.0, 1.0);
        return;
    }

    MaterialData m = materials[mi];

    vec3 N = normalize(Normal);
    vec3 V = normalize(globals[0].cameraPos - FragPos);

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
