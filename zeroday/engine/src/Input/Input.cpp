#include "Input/Input.h"

namespace Zeroday::Input {

	bool firstMouse = true;
	bool g_IsKeyPressed[KEY_MAX] = {};
	bool g_LastFrameIsKeyPressed[KEY_MAX] = {};
	bool g_IsKeyDown[KEY_MAX] = {};
	bool g_IsKeyHeld[KEY_MAX] = {};
	bool g_IsKeyUp[KEY_MAX] = {};

	float yaw = -90.0f;
	float pitch = 0.0f;

	double currMousePosX = 0.0f;
	double currMousePosY = 0.0f;
	double lastMousePosX = 0.0f;
	double lastMousePosY = 0.0f;

	double sensitivity = 0.3f;

	double mouseDeltaX = 0.0f;
	double mouseDeltaY = 0.0f;

	void Update() {
		for (int i = 0; i < KEY_MAX; i++) {
			// If last frame and the key is currently pressed => held
			g_IsKeyHeld[i] = g_LastFrameIsKeyPressed[i] && g_IsKeyPressed[i];

			// If last frame was pressed but not anymore => key released
			g_IsKeyUp[i] = g_LastFrameIsKeyPressed[i] && !g_IsKeyPressed[i];

			// If last frame was not printed but now it is => newly printed
			g_IsKeyDown[i] = !g_LastFrameIsKeyPressed[i] && g_IsKeyPressed[i];

			// The state of this frame will be the previous state in the next frame
			g_LastFrameIsKeyPressed[i] = g_IsKeyPressed[i];
		}
	}

	void UpdateMouseDelta() {
		// to avoid a sudden change of the initial mouse delta
		if (firstMouse) {
			// update last mouse pos
			lastMousePosX = currMousePosX;
			lastMousePosY = currMousePosY;
			
			firstMouse = false;
			mouseDeltaX = 0.0;
			mouseDeltaY = 0.0;
			return;
		}

		// delta = last frame - current frame
		mouseDeltaX = currMousePosX - lastMousePosX;

		// Y-axis should be (curr - last) because Y axis in the opposite direction
		mouseDeltaY = lastMousePosY- currMousePosY;

		mouseDeltaX *= sensitivity;
		mouseDeltaY *= sensitivity;

		yaw   += static_cast<float>(mouseDeltaX);
		pitch += static_cast<float>(mouseDeltaY);
		if (pitch > 89.0) pitch = 89.0;
		if (pitch < -89.0) pitch = -89.0;

		lastMousePosX = currMousePosX;
		lastMousePosY = currMousePosY;
	}

	void ResetMouseDelta() {
		mouseDeltaX = 0.0;
		mouseDeltaY = 0.0;
	}
}
