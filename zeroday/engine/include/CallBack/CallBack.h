#pragma once
#include "Core/Window.h"
#include "Input/Input.h"

namespace Zeroday::CallBack {
	void InitResources(GLFWwindow* window) noexcept;
	void FrameBufferSizeCallback(GLFWwindow* window, int width, int height) noexcept;
	void Key_CallBack(GLFWwindow* window, int key, int scancode, int action, int mods) noexcept;
	void Mouse_CallBack(GLFWwindow* window, double xpos, double ypos) noexcept;
	//void scroll_CallBack(GLFWwindow* window, double xoffset, double yoffset);
}