#pragma once
#include "core/Window.h"
#include "Input/Input.h"


namespace CallBack
{
	void initResources(GLFWwindow* window) noexcept;
	void frameBufferSizeCallback(GLFWwindow* window, int width, int height) noexcept;
	void key_CallBack(GLFWwindow* window, int key, int scancode, int action, int mods) noexcept;
	void mouse_CallBack(GLFWwindow* window, double xpos, double ypos) noexcept;
	//void scroll_CallBack(GLFWwindow* window, double xoffset, double yoffset);
}