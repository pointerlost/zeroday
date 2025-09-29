#include "Core/Window.h"
#include "Core/EngineConfig.h"
#include <cstring>

#include "Core/Logger.h"

namespace Zeroday {

	Window::Window() : width(SCR_WIDTH), height(SCR_HEIGHT),
		title("why do we fall, so we can learn to pick ourselves up!"),
		monitor(nullptr), share(nullptr), mode(nullptr)
	{
		window = nullptr;
	}

	bool Window::InitResources() {
		if (!glfwInit()) {
			Warn("glfwInit failed!");
			return false;
		}

		SetUpGLFWmonitorVidMode();
		SetUpGLFWContext();
		
		window = glfwCreateWindow(mode->width, mode->height, title, monitor, NULL);

		if (!window) {
			Warn("glfwCreateWindow failed!");
			return false;
		}

		// GLFW doesn't leave the window's OpenGL context current when glfwCreateWindow() succeeds!
		glfwMakeContextCurrent(window);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		//glfwSetCursorPos(window, width / 2, height / 2);

		LoadGlad();
		CheckOpenGLVersion();

		return true;
	}

	GLFWwindow* Window::GetGLFWwindow() const {
		return window;
	}

	float Window::GetWidth() const {
		return width;
	}

	float Window::GetHeight() const {
		return height;
	}

	void Window::SetUpGLFWmonitorVidMode() {
		monitor = glfwGetPrimaryMonitor();

		if (!monitor) {
			Warn("glfwGetPrimaryMonitor failed!");
		}

		mode = glfwGetVideoMode(monitor);

		if (!mode) {
			Warn("glfwGetVideoMode failed!");
		}

		glfwWindowHint(GLFW_RED_BITS, mode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
	}

	void Window::SetUpGLFWContext() {
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	}

	bool Window::LoadGlad() {
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			Warn("gladLoadGLLoader failed!");
			return false;
		}
		return true;
	}

	bool Window::CheckOpenGLVersion() {
		// Get version info
		GLint major, minor;
		glGetIntegerv(GL_MAJOR_VERSION, &major);
		glGetIntegerv(GL_MINOR_VERSION, &minor);

		Info("OpenGL version: " + std::to_string(major) + "." + std::to_string(minor));
		Info("OpenGL vendor: " + std::string(reinterpret_cast<const char*>(glGetString(GL_VENDOR))));
		Info("OpenGL renderer: " + std::string(reinterpret_cast<const char*>(glGetString(GL_RENDERER))));

		// Check if we have at least OpenGL 4.4
		if (major < 4 || (major == 4 && minor < 4)) {
			Warn("OpenGL 4.4 is not supported! Your version: "
				+ std::to_string(major) + "." + std::to_string(minor));
			return false;
		}

		GLint n = 0;
		glGetIntegerv(GL_NUM_EXTENSIONS, &n);

		for (GLint i = 0; i < n; i++) {
			if (const auto ext = reinterpret_cast<const char *>(glGetStringi(GL_EXTENSIONS, i)); strcmp(ext, "GL_ARB_bindless_texture") == 0) {
				Info("Bindless textures supported!");
				return true;
			}
		}
		Warn("GL_ARB_bindless_texture is not supported!");
		return false;
	}
}
