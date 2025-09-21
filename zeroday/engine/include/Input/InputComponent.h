#pragma once
#include <iostream>
#include <memory>
#include "Input/InputContext.h"
#include <vector>

namespace Zeroday { class Transform; };
namespace Zeroday		 { class Scene;			  };
namespace LIGHTING   { class Light;			  };

namespace Input
{

	// Enum to identify the type of input component
	enum struct InputType
	{
		SphereInputComponent,
		CubeInputComponent,
		CircleInputComponent,
		LightInputComponent,
	};

	// This is a Interface class for Input components!
	class IInputComponent
	{
	public:
		IInputComponent() = default;

		virtual ~IInputComponent() = default;

		virtual void processInput(Zeroday::Scene& scene) = 0;

		virtual void setUpInputContext() = 0;

		virtual void changeInputObjectWithPress(uint32_t lightVecSize) = 0;

		virtual InputType getInputType() const = 0;
	};


	class SphereInputComponent : public IInputComponent
	{
	public:
		SphereInputComponent(std::shared_ptr<Zeroday::Transform> transform, Input::InputContext context);
		~SphereInputComponent() = default;

		void processInput(Zeroday::Scene& scene) override;

		void setUpInputContext() override;

		void changeInputObjectWithPress(uint32_t lightVecSize) override;

		InputType getInputType() const override { return m_inputType; }

	private:
		std::shared_ptr<Zeroday::Transform> m_transformation;
		Input::InputContext m_dataContext;

		int m_activeLightComponentIdx = 0;
		std::vector<bool> m_inputSelectors;

		InputType m_inputType = InputType::SphereInputComponent;

		bool m_isRotating = false;
	};


	class CubeInputComponent : public IInputComponent
	{
	public:
		CubeInputComponent(std::shared_ptr<Zeroday::Transform> transform, Input::InputContext context);
		~CubeInputComponent() = default;

		void processInput(Zeroday::Scene& scene) override;

		void setUpInputContext() override;

		void changeInputObjectWithPress(uint32_t lightVecSize) {};

		InputType getInputType() const override { return m_inputType; }

	private:
		std::shared_ptr<Zeroday::Transform> m_transformation;

		InputType m_inputType = InputType::CubeInputComponent;

		Input::InputContext m_dataContext;
	};


	class CircleInputComponent : public IInputComponent
	{
	public:
		CircleInputComponent(std::shared_ptr<Zeroday::Transform> transform, Input::InputContext context);

		void processInput(Zeroday::Scene& scene) override {};

		void setUpInputContext() override;

		void changeInputObjectWithPress(uint32_t lightVecSize) {};

		InputType getInputType() const override { return m_inputType; }

	private:
		std::shared_ptr<Zeroday::Transform> m_transformation;

		InputType m_inputType = InputType::CircleInputComponent;

		Input::InputContext m_dataContext;
	};


	class LightInputComponent : public IInputComponent
	{
	public:
		LightInputComponent(std::shared_ptr<Zeroday::Transform> transform, Input::InputContext context, std::shared_ptr<LIGHTING::Light>& myLight);

		void processInput(Zeroday::Scene& scene) override;

		void setUpInputContext() override;

		void changeInputObjectWithPress(uint32_t lightVecSize) override;

		InputType getInputType() const override { return m_inputType; }
		
		void circularMotion(std::shared_ptr<LIGHTING::Light>& light);

		void moveOnPress(std::shared_ptr<LIGHTING::Light>& light);

	private:
		std::shared_ptr<Zeroday::Transform> m_transformation;
		Input::InputContext m_dataContext;

		InputType m_inputType = InputType::LightInputComponent;

		std::shared_ptr<LIGHTING::Light> m_light;

		int m_activeLightComponentIdx = 0;
		std::vector<bool> m_inputSelectors;

		bool m_isRotating = false;
	};

}
