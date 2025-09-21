#pragma once
#include <memory>
#include <glm/ext.hpp>
#include <nlohmann/detail/meta/is_sax.hpp>
#include "Light.h"

namespace Zeroday {

	class Light {
	public:
		virtual ~Light() = default;

		void SetRadiance(const glm::vec3& diffuse) { m_Radiance = diffuse; }
		[[nodiscard]] const glm::vec3& GetRadiance() const { return m_Radiance; }

		void SetIntensity(float i) { m_Intensity = i; }
		[[nodiscard]] float GetIntensity() const { return m_Intensity; }

		virtual void SetPosition(const glm::vec3& position) = 0;
		virtual glm::vec3 GetPosition() const = 0;

	protected:
		glm::vec3 m_Radiance = glm::vec3(1.0);
		float m_Intensity = 1.0f;
	};

	class DirectionalLight final : public Light {
	public:
		void SetDirection(const glm::vec3& direction) { m_Direction = direction; }
		[[nodiscard]] const glm::vec3& GetDirection() const { return m_Direction; }

		void SetPosition(const glm::vec3&) override {} // Directional lights don't have position
		glm::vec3 GetPosition() const override { return glm::vec3(0.0f); }

	private:
		glm::vec3 m_Direction = glm::vec3(0.0, 0.0, 1.0);
	};

	class PointLight : public Light {
	public:
		void SetPosition(const glm::vec3& position) override { m_Position = position; }
		[[nodiscard]] glm::vec3 GetPosition() const override { return m_Position; }

		void SetConstant(float constant) { m_Constant = constant; }
		void SetLinear(float linear) { m_Linear = linear; }
		void SetQuadratic(float quadratic) { m_Quadratic = quadratic; }

	private:
		glm::vec3 m_Position = glm::vec3(0.0);
		float m_Constant = 1.0f;
		float m_Linear = 0.014f;
		float m_Quadratic = 0.0007f;
	};

	class SpotLight final : public PointLight {
	public:
		void SetCutOff(float cutOff) { m_CutOff = cutOff; }
		void SetOuterCutOff(float outerCutOff) { m_OuterCutOff = outerCutOff; }
		void SetDirection(const glm::vec3& direction) { m_Direction = direction; }

	private:
		glm::vec3 m_Direction = glm::vec3(0.0, 0.0, 1.0);
		float m_CutOff = static_cast<float>(glm::cos(glm::radians(12.5)));
		float m_OuterCutOff = static_cast<float>(glm::cos(glm::radians(17.5)));
	};

}
