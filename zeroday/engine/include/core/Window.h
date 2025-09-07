#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


namespace core {

	class Window {
	public:
		Window();

		bool initResources();

		[[nodiscard]] GLFWwindow* getGLFWwindow() const;
		[[nodiscard]] float getWidth()  const;
		[[nodiscard]] float getHeight() const;

		void SetUpGLFWmonitorVidmode();
		void SetUpGLFWContext();
		bool loadGlad();

		bool checkOpenGLVersion();

	private:

		// SCREEN STUFF
		float width;
		float height;
		const char* title;

		// GLFW STUFF
		GLFWwindow* window;
		GLFWmonitor* monitor;
		GLFWwindow* share;
		const GLFWvidmode* mode;
	};
}
