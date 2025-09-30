#pragma once
#include <iostream>

namespace Zeroday::Input {
	
	struct InputContext
	{
	public:
		float m_delta = {};
		float m_rotationSpeed = {};
		float m_currentTime = {};
		float m_lastTime = {};
		float m_angle = {};
		float m_interval = {};
		float m_radius = {};

		InputContext() = default;
		InputContext(const InputContext&) = default;
		InputContext(InputContext&&) = default;
		InputContext& operator=(const InputContext&) = default;
		InputContext& operator=(InputContext&&) = default;
	};

}

