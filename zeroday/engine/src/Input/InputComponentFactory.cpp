#include "Input/InputComponentFactory.h"
#include <Input/InputComponent.h>
#include <core/Logger.h>
#include <Graphics/OpenGL/Transformations/Transformations.h>
#include <Graphics/OpenGL/Lighting/Light.h>

namespace Input
{
	std::shared_ptr<IInputComponent> InputComponentFactory::createObjectComponent(InputType type, std::shared_ptr<Zeroday::Transform> transform)
	{
		InputContext context{};

		switch (type)
		{
		case InputType::CubeInputComponent:
			return std::make_shared<CubeInputComponent>(transform, context);
		case InputType::SphereInputComponent:
			return std::make_shared<SphereInputComponent>(transform, context);
		default:
			Zeroday::Error("Unknown input component type: " + std::to_string(static_cast<int>(type)));
			return nullptr;
		}
	}

	std::shared_ptr<IInputComponent> InputComponentFactory::createLightComponent(
		InputType type,
		std::shared_ptr<Zeroday::Transform> transform,
		std::shared_ptr<LIGHTING::Light>& light)
	{
		InputContext context{};
	
		return std::make_shared<LightInputComponent>(transform, context, light);
	}
} // namespace Input