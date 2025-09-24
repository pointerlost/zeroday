#pragma once
#include <string>
#include "Scene/Entity.h"
#include "Graphics/OpenGL/Lighting/Light.h"

namespace Zeroday
{
	class SceneObjectFactory
	{
	public:
		explicit SceneObjectFactory(Scene& scene);
		~SceneObjectFactory() = default;

		[[nodiscard]] Entity CreatePrimitiveObject(const std::string& meshName = "cube") const;

		[[nodiscard]] Entity CreateLight(opengl::LightType type = opengl::LightType::Point, const std::string& meshName = "sphere") const;

		[[nodiscard]] Entity CreateCamera(CameraMode mode = CameraMode::Perspective);

		[[nodiscard]] Entity CreateModel(const std::string& path) const;

	private:
		Scene& m_Scene;
	};
}
