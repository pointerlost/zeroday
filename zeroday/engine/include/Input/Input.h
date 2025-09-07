#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Input
{

	constexpr int KEY_MAX = GLFW_KEY_LAST + 1; // 349 key

	extern double xPosMouse;
	extern double yPosMouse;
	extern double lastMousePosX;
	extern double lastMousePosY;

	extern double sensitivity;
	
	extern double mouseDeltaX;
	extern double mouseDeltaY;

	extern float deltaTime; // Time between current frame and last frame
	extern float lastFrame;

	extern bool g_IsKeyPressed[KEY_MAX];
	extern bool g_LastFrameIsKeyPressed[KEY_MAX];

	extern bool g_IsKeyDown[KEY_MAX];

	extern bool g_IsKeyHeld[KEY_MAX];

	extern bool g_IsKeyUp[KEY_MAX];
	
	void update();

	void updateMouseDelta();

	void ResetMouseDelta();
}

