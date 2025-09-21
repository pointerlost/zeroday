#include "core/Window.h"
#include <cstring>
#include "core/EngineConfig.h"

namespace Zeroday {

	Window::Window() :
		width(SCR_WIDTH), height(SCR_HEIGHT), title("why do we fall, so we can learn to pick ourselves up!"), monitor(nullptr), share(nullptr), mode(nullptr)
	{
		window = nullptr;
	}

	bool Window::initResources()
	{
		if (!glfwInit()) {
			std::cerr << "glfwInit failed!" << std::endl;
			return false;
		}

		SetUpGLFWmonitorVidmode();
		SetUpGLFWContext();
		
		window = glfwCreateWindow(mode->width, mode->height, title, monitor, NULL);

		if (!window) {
			std::cerr << "glfwCreateWindow failed!" << std::endl;
			return false;
		}

		// GLFW doesn't leave the window's OpenGL context current when glfwCreateWindow() succeeds!
		glfwMakeContextCurrent(window);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		//glfwSetCursorPos(window, width / 2, height / 2);

		loadGlad();
		checkOpenGLVersion();

		/*std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
		std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

		int profile;
		glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &profile);
		if (profile & GL_CONTEXT_CORE_PROFILE_BIT)
			std::cout << "Core profile\n";
		else if (profile & GL_CONTEXT_COMPATIBILITY_PROFILE_BIT)
			std::cout << "Compatibility profile\n";*/

		return true;
	}

	GLFWwindow* Window::getGLFWwindow() const
	{
		return window;
	}

	float Window::getWidth() const
	{
		return width;
	}

	float Window::getHeight() const
	{
		return height;
	}

	void Window::SetUpGLFWmonitorVidmode()
	{
		monitor = glfwGetPrimaryMonitor();

		if (!monitor) {
			std::cerr << "monitor can't be nullptr for fullscreen mode (from core::Window) !\n";
		}

		mode = glfwGetVideoMode(monitor);

		if (!mode) {
			std::cerr << "vidmode can't be nullptr for fullscreen mode (from core::Window) !\n";
		}

		glfwWindowHint(GLFW_RED_BITS, mode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
	}

	void Window::SetUpGLFWContext()
	{
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	}

	bool Window::loadGlad()
	{
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cerr << "gladLoadGLLoader failed!" << std::endl;
			return false;
		}

		return true;
	}

	bool Window::checkOpenGLVersion()
	{
		// Get version info
		GLint major, minor;
		glGetIntegerv(GL_MAJOR_VERSION, &major);
		glGetIntegerv(GL_MINOR_VERSION, &minor);

		std::cout << "OpenGL version: " << major << "." << minor << std::endl;
		std::cout << "OpenGL vendor: " << glGetString(GL_VENDOR) << std::endl;
		std::cout << "OpenGL renderer: " << glGetString(GL_RENDERER) << std::endl;

		// Check if we have at least OpenGL 4.4
		if (major < 4 || (major == 4 && minor < 4)) {
			std::cerr << "OpenGL 4.4 is not supported! Your version: "
					  << major << "." << minor << std::endl;
			return false;
		}

		GLint n = 0;
		glGetIntegerv(GL_NUM_EXTENSIONS, &n);

		for (GLint i = 0; i < n; i++) {
			if (const auto ext = reinterpret_cast<const char *>(glGetStringi(GL_EXTENSIONS, i)); strcmp(ext, "GL_ARB_bindless_texture") == 0) {
				std::cout << "Bindless textures supported!\n";
				return true;
			}
		}
		std::cerr << "Bindless textures NOT supported on this GPU!\n";
		return false;

		return true;
	}
}
