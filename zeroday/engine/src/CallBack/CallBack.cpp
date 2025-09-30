#include "CallBack/CallBack.h"

namespace Zeroday::CallBack
{
	void InitResources(GLFWwindow* window) noexcept
	{
		glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);
		glfwSetKeyCallback(window, Key_CallBack);
		glfwSetCursorPosCallback(window, Mouse_CallBack);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		//glfwSetScrollCallback(window, scroll_CallBack);
	}

	void FrameBufferSizeCallback(GLFWwindow* window, int width, int height) noexcept
	{
		glViewport(0, 0, width, height);
	}

	void Key_CallBack(GLFWwindow* window, int key, int scancode, int action, int mods) noexcept
	{
		if (key < 0 || key >= Input::KEY_MAX) return;

		using namespace Input;

		switch (action)
		{
		case GLFW_PRESS:
			g_IsKeyPressed[key] = true;
			break;

		case GLFW_RELEASE:
			g_IsKeyPressed[key] = false;
			break;
		}
	}

	void Mouse_CallBack(GLFWwindow* window, double xpos, double ypos) noexcept
	{
		Input::xPosMouse = xpos;
		Input::yPosMouse = ypos;
		Input::UpdateMouseDelta();
	}
	
}