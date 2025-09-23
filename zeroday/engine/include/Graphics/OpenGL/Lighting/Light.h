#pragma once
#include <memory>
#include <glm/ext.hpp>
#include <nlohmann/detail/meta/is_sax.hpp>
#include "Light.h"
#include "Graphics/OpenGL/Macros.h"

namespace Zeroday::opengl {

    enum class LightType {
        Directional = 0,
        Point = 1,
        Spot = 2,
    };

    class Light {
    public:
        explicit Light(LightType type = LightType::Point);

        void SetType(LightType type) { m_Type = type; }
        [[nodiscard]] LightType GetType() const { return m_Type; }
        void SetPosition(const glm::vec3& position) { m_Position = position; }
        [[nodiscard]] const glm::vec3& GetPosition() const { return m_Position; }
        void SetDirection(const glm::vec3& direction) { m_Direction = glm::normalize(direction); }
        [[nodiscard]] const glm::vec3& GetDirection() const { return m_Direction; }
        void SetRadiance(const glm::vec3& radiance) { m_Radiance = radiance; }
        [[nodiscard]] const glm::vec3& GetRadiance() const { return m_Radiance; }
        void SetIntensity(float intensity) { m_Intensity = intensity; }
        [[nodiscard]] float GetIntensity() const { return m_Intensity; }
        void SetConstant(float constant) { m_Constant = constant; }
        [[nodiscard]] float GetConstant() const { return m_Constant; }
        void SetLinear(float linear) { m_Linear = linear; }
        [[nodiscard]] float GetLinear() const { return m_Linear; }
        void SetQuadratic(float quadratic) { m_Quadratic = quadratic; }
        [[nodiscard]] float GetQuadratic() const { return m_Quadratic; }
        void SetCutOff(float cutOff) { m_CutOff = cutOff; }
        [[nodiscard]] float GetCutOff() const { return m_CutOff; }
        void SetOuterCutOff(float outerCutOff) { m_OuterCutOff = outerCutOff; }
        [[nodiscard]] float GetOuterCutOff() const { return m_OuterCutOff; }

        static Light CreateDirectional(const glm::vec3& direction, const glm::vec3& color, float intensity);

        static Light CreatePoint(const glm::vec3& position, const glm::vec3& color, float intensity);

        static Light CreateSpot(const glm::vec3& position, const glm::vec3& direction,
                                const glm::vec3& color, float intensity, float angle);

        [[nodiscard]] LightSSBO ToGPUFormat() const;

    private:
        LightType m_Type = LightType::Point; // 0=directional, 1=point, 2=spot
        glm::vec3 m_Position  = glm::vec3(0.0f);
        glm::vec3 m_Direction = glm::vec3(0.0f, -1.0f, 0.0f);
        glm::vec3 m_Radiance  = glm::vec3(1.0f);
        float m_Intensity = 1.0f;
        float m_Constant  = 1.0f;
        float m_Linear    = 0.09f;
        float m_Quadratic = 0.032f;
        float m_CutOff      = glm::cos(glm::radians(12.5f));
        float m_OuterCutOff = glm::cos(glm::radians(17.5f));
    };

}
