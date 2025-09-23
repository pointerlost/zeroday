#include "Graphics/OpenGL/Lighting/Light.h"

namespace Zeroday::opengl {
    Light::Light(LightType type) {
    }

    Light Light::CreateDirectional(const glm::vec3 &direction, const glm::vec3 &color, float intensity) {
        Light light;
        light.m_Type = LightType::Directional;
        light.m_Direction = glm::normalize(direction);
        light.m_Radiance  = color;
        light.m_Intensity = intensity;

        return light;
    }

    Light Light::CreatePoint(const glm::vec3 &position, const glm::vec3 &color, float intensity) {
        Light light;
        light.m_Type = LightType::Point;
        light.m_Position = position;
        light.m_Radiance = color;
        light.m_Intensity = intensity;

        return light;
    }

    Light Light::CreateSpot(const glm::vec3 &position, const glm::vec3 &direction, const glm::vec3 &color,
                            float intensity, float angle) {
        Light light;
        light.m_Type = LightType::Spot;
        light.m_Position  = position;
        light.m_Direction = glm::normalize(direction);
        light.m_Radiance  = color;
        light.m_Intensity = intensity;
        light.m_CutOff      = glm::cos(glm::radians(angle));
        light.m_OuterCutOff = glm::cos(glm::radians(angle + 5.0f)); // +5 degree falloff

        return light;
    }

    LightSSBO Light::ToGPUFormat() const {
        LightSSBO gpuLight;

        gpuLight.type = static_cast<int>(m_Type);
        gpuLight.position  = m_Position;
        gpuLight.direction = m_Direction;
        gpuLight.radiance  = m_Radiance * m_Intensity;
        gpuLight.intensity = m_Intensity;
        gpuLight.constant  = m_Constant;
        gpuLight.linear    = m_Linear;
        gpuLight.quadratic = m_Quadratic;
        gpuLight.cutOff    = m_CutOff;
        gpuLight.outerCutOff = m_OuterCutOff;

        return gpuLight;
    }

}
