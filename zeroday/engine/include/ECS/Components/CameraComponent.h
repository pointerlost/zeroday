#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <core/EngineConfig.h>
#include <memory>

namespace ECS
{
	struct TransformComponent;

	enum Camera_Movement
	{
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};

	// !! NEED UPDATE !!
	enum CameraType {
		ORTHO,
		PERSPECTIVE
	};

	enum class CameraMode {
		Primary, // FPS-style
		Orbit    // Editor-style
	};

	struct CameraComponent
	{
		explicit CameraComponent(CameraMode m = CameraMode::Primary, CameraType t = PERSPECTIVE) : mode(m), type(t) {}
		CameraComponent(const CameraComponent&) = default;
		CameraComponent(CameraComponent&&) = default;
		CameraComponent& operator=(CameraComponent&&) = default;
		CameraComponent& operator=(const CameraComponent&) = default;

		void setMode(CameraMode m) { mode = m; }
		void setType(CameraType t) { type = t; }
		[[nodiscard]] CameraMode getMode() const { return mode; }
		[[nodiscard]] CameraType getType() const { return type; }

		void SetPosition(const glm::vec3& pos)             { m_position = pos;  }
		[[nodiscard]] const glm::vec3& GetPosition() const { return m_position; }

		void setFront(float x, float y, float z)         { m_front = glm::vec3{ x,y,z }; };
		[[nodiscard]] const glm::vec3& getFront() const  { return m_front;               };
		[[nodiscard]] const glm::vec3& getUp()    const  { return m_up;                  };

		void setYaw(float y)               { yaw += y;   };
		[[nodiscard]] float getYaw() const { return yaw; };

		void setPitch(float p) { pitch = std::clamp(pitch + p, -88.0f, 88.0f); };
		[[nodiscard]] float getPitch() const { return pitch; };

		[[nodiscard]] glm::mat4& GetViewMatrix();
		[[nodiscard]] const glm::mat4& GetViewMatrix() const;

		void setProjectionMatrix(float fv, float near, float far);
		[[nodiscard]] const glm::mat4& GetProjectionMatrix() const { return projectionMatrix; };
		[[nodiscard]] glm::mat4& GetProjectionMatrix() { return projectionMatrix; };

		void updateVectors();
		void update();

		void setFov(float fv)         { fov = fv;         };
		void setNearPlane(float near) { nearPlane = near; };
		void setFarPlane(float far)   { farPlane = far;   };
		[[nodiscard]] float getFov()       const { return fov;       };
		[[nodiscard]] float getNearPlane() const { return nearPlane; };
		[[nodiscard]] float getFarPlane()  const { return farPlane;  };

	private:
		CameraMode mode;
		CameraType type;
		float orbitDistance = 6.0f;

		glm::vec3 m_position = glm::vec3(0.0f, 0.0f, -6.0f);
		glm::vec3 m_target{};
		glm::vec3 m_right{};
		glm::vec3 m_up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 m_front{};
		glm::vec3 m_worldUp = glm::vec3{ 0.0f, 1.0f, 0.0f };

		float yaw = -90.0f;
		float pitch = 0.0f;

		float fov = 60.0f;
		float nearPlane = 0.3f;
		float farPlane = 5000.0f;
		float aspectRatio = SCR_WIDTH / SCR_HEIGHT;

		// viewMatrix is used only for Primary mode; Orbit mode computes on the fly in getViewMatrix()
		mutable glm::mat4 viewMatrix{};
		mutable glm::mat4 projectionMatrix{};
	};
}

