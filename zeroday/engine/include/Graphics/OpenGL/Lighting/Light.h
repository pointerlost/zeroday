#pragma once
#include <memory>
#include <glm/ext.hpp>
#include <nlohmann/detail/meta/is_sax.hpp>
#include "Light.h"

namespace LIGHTING
{
	enum class LightType : int
	{
		Directional = 0,
		Point		= 1,
		Spot		= 2
	};

	class Light
	{
	public:
		explicit Light(const LightType type) : m_type(type) {};

		void setCutOff(float cutOff)                  { if (m_cutOff != cutOff) { m_cutOff = cutOff; } };
		[[nodiscard]] float getCutOff()	     const    { return m_cutOff;       };

		void setOuterCutOff(float outerCutOff)        { if (m_outerCutOff != outerCutOff) { m_outerCutOff = outerCutOff; } };
		[[nodiscard]] float getOuterCutOff() const    { return m_outerCutOff;  };

		void setConstant(float constant)              { if (m_constant != constant) { m_constant = constant; } };
		[[nodiscard]] float getConstant()    const    { return m_constant;     };

		void setLinear(float linear)                  { if (m_linear != linear) { m_linear = linear; } };
		[[nodiscard]] float getLinear()	     const    { return m_linear;	   };

		void setQuadratic(float quadratic)            { if (m_quadratic != quadratic) { m_quadratic = quadratic; } };
		[[nodiscard]] float getQuadratic()   const    { return m_quadratic;    };

		void setRadiance(const glm::vec3& diffuse)    { if (m_radiance != diffuse) { m_radiance = diffuse; } };
		[[nodiscard]] glm::vec3& getRadiance()         { return m_radiance;     };

		void setIntensity(float i)                    { m_intensity = i;       };
		[[nodiscard]] float getIntensity()   const    { return m_intensity;    };

		void setDirection(const glm::vec3& direction) { if (m_direction != direction) { m_direction = direction; } };
		[[nodiscard]] glm::vec3& getDirection()       { return m_direction;    };

		void setPosition(const glm::vec3& position)   { if (m_position != position) { m_position = position; } };
		[[nodiscard]] glm::vec3& getPosition()        { return m_position;     };

		[[nodiscard]] LightType getType()      const  { return m_type;         };

	private:
		// Light type, directional = 0, point = 1, spot = 2
		LightType m_type;

		// for scene lighting not light object material properties
		glm::vec3 m_radiance  = glm::vec3(1.0);
		float m_intensity = 1.0f;

		glm::vec3 m_position = glm::vec3(0.0);
		glm::vec3 m_direction = glm::vec3(0.0, 0.0, 1.0);

		// spotlight soft edge
		float m_cutOff		= static_cast<float>(glm::cos(glm::radians(12.5)));
		float m_outerCutOff = static_cast<float>(glm::cos(glm::radians(17.5)));

		// attenuation parameters
		float m_constant  = 1.0f;
		float m_linear	  = 0.014f;
		float m_quadratic = 0.0007f;
	};
}
