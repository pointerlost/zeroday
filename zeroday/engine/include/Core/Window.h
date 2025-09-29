#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


namespace Zeroday {

	class Window {
	public:
		Window();

		bool InitResources();

		[[nodiscard]] GLFWwindow* GetGLFWwindow() const;
		[[nodiscard]] float GetWidth()  const;
		[[nodiscard]] float GetHeight() const;

		void SetUpGLFWmonitorVidMode();
		void SetUpGLFWContext();
		bool LoadGlad();

		[[nodiscard]] bool CheckOpenGLVersion();

	private:
		float width;
		float height;
		const char* title;

		GLFWwindow* window;
		GLFWmonitor* monitor;
		GLFWwindow* share;
		const GLFWvidmode* mode;
	};
}
