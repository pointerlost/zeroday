#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

namespace ECS
{
	enum class CameraType {
		Perspective,
		Orthographic
	};

	struct CameraComponent {
		explicit CameraComponent(CameraType t) : type(t) {}

		float fov = 45.0f;
		float aspectRatio = 16.0f / 9.0f;
		float nearPlane = 0.1f;
		float farPlane = 100.0f;

		// Camera type
		CameraType type = CameraType::Perspective;

		// For orthographic cameras
		float orthoSize = 10.0f;

		[[nodiscard]] glm::mat4 GetProjectionMatrix() const {
			if (type == CameraType::Perspective) {
				return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
			}

			// Orthographic
			const float halfWidth = orthoSize * aspectRatio * 0.5f;
			const float halfHeight = orthoSize * 0.5f;
			return glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, nearPlane, farPlane);
		}
	};
}

