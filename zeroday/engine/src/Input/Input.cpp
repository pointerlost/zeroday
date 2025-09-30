#include "Input/Input.h"

namespace Zeroday::Input {

	bool firstMouse = true;
	bool g_IsKeyPressed[KEY_MAX] = {};
	bool g_LastFrameIsKeyPressed[KEY_MAX] = {};
	bool g_IsKeyDown[KEY_MAX] = {};
	bool g_IsKeyHeld[KEY_MAX] = {};
	bool g_IsKeyUp[KEY_MAX] = {};

	double xPosMouse = 0.0f;
	double yPosMouse = 0.0f;
	double lastMousePosX = 0.0f;
	double lastMousePosY = 0.0f;

	double sensitivity = 0.0444f;

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
			lastMousePosX = xPosMouse;
			lastMousePosY = yPosMouse;
			
			firstMouse = false;
			mouseDeltaX = 0.0;
			mouseDeltaY = 0.0;
			return;
		}

		// delta = last frame - current frame
		mouseDeltaX = xPosMouse - lastMousePosX;

		// Y axis should be (curr - last) because Y axis in the opposite direction
		mouseDeltaY = lastMousePosY- yPosMouse;

		lastMousePosX = xPosMouse;
		lastMousePosY = yPosMouse;
	}

	void ResetMouseDelta() {
		mouseDeltaX = 0.0;
		mouseDeltaY = 0.0;
	}
}
