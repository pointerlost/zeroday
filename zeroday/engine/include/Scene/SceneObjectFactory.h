#pragma once
#include <string>
#include <memory>
#include "Scene/Entity.h"
#include "Graphics/OpenGL/Lighting/Light.h"

namespace ecs      { class World;         }
namespace opengl   { class RenderContext; }

namespace Zeroday
{
	class SceneObjectFactory
	{
	public:
		explicit SceneObjectFactory(ecs::World& w);
		~SceneObjectFactory() = default;

		[[nodiscard]] ecs::Entity CreatePrimitiveObject(const std::string& meshName = "cube") const;

		[[nodiscard]] ecs::Entity CreateLight(LIGHTING::LightType type = LIGHTING::LightType::Point, const std::string& meshName = "sphere") const;

		[[nodiscard]] ecs::Entity CreateCamera(ecs::CameraType type = ecs::CameraType::Perspective);

		[[nodiscard]] ecs::Entity CreateModel(const std::string& path) const;

	private:
		ecs::World& world;
	};
}
