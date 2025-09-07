#pragma once
#include <string>
#include <memory>
#include "ECS/Entity.h"
#include "ECS/Components/CameraComponent.h"
#include "Graphics/OpenGL/Lighting/Light.h"

namespace ECS      { class World;         }
namespace OpenGL   { class RenderContext; }

namespace Graphics
{
	class SceneObjectFactory
	{
	public:
		explicit SceneObjectFactory(ECS::World& w);
		~SceneObjectFactory() = default;

		[[nodiscard]] ECS::Entity CreatePrimitiveObject(const std::string& meshName = "cube") const;

		[[nodiscard]] ECS::Entity CreateLight(LIGHTING::LightType type = LIGHTING::LightType::Point, const std::string& meshName = "sphere") const;

		[[nodiscard]] ECS::Entity CreateCamera(ECS::CameraMode mode = ECS::CameraMode::Primary, ECS::CameraType type = ECS::PERSPECTIVE);

		[[nodiscard]] ECS::Entity CreateModel(const std::string& path) const;

	private:
		ECS::World& world;
	};
}
