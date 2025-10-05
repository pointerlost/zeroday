#pragma once
#include <GLFW/glfw3.h>

namespace Zeroday::Input {

	constexpr int KEY_MAX = GLFW_KEY_LAST + 1; // 349 key

	extern double currMousePosX;
	extern double currMousePosY;
	extern double lastMousePosX;
	extern double lastMousePosY;

	extern float yaw;
	extern float pitch;

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
	
	void Update();

	void UpdateMouseDelta();

	void ResetMouseDelta();
}

