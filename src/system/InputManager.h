#include <GLFW/glfw3.h>
#include <algorithm>
#include <glm/glm.hpp>
#include <iostream>
#include "graphics/Transform.h"
#include "Log.h"

#define NUM_KEYS 1024
#define NUM_MOUSE_BUTTONS 8
#define GAMEPAD_DEAD_ZONE 0.1f

#define MAX_GAMEPAD_AXES 6
#define MAX_GAMEPAD_BUTTONS 14

#pragma once
class InputManager
{
public:

	static bool keys[NUM_KEYS];
	static bool mouseButtons[NUM_MOUSE_BUTTONS];
	static double mouseX;
	static double mouseY;

	// Simply if a key is pressed
	static bool GetKey(int key);
	// If a key was just pressed (first frame)
	static bool GetKeyDown(int key);
	// If a key was just released
	static bool GetKeyUp(int key);

	// Simply if a mouse button is pressed
	static bool GetMouseButton(int button);
	// If a mouse button was just pressed (first frame)
	static bool GetMouseButtonDown(int button);
	// If a mouse button was just released
	static bool GetMouseButtonUp(int button);
	
	// Gets raw float value of axis
	static float GetGamepadAxisRaw(int axis);
	// Returns 0 if axis isn't farther than deadzone, otherwise returns direction (-1,1)
	static float GetGamepadAxisDigital(int axis);
	static bool GetGamepadButton(int button);
	static bool GetGamepadButtonDown(int button);
	static bool GetGamepadButtonUp(int button);

	// Convert pixel coordinates to world coordinates
	static glm::vec2 GetWorldMousePos(int windowWidth, int windowHeight, float camRight, const Transform& camTransform);
	// Call at the end of a frame
	static void Update();

private:
	static bool lastKeys[NUM_KEYS];
	static bool lastMouseButtons[NUM_MOUSE_BUTTONS];
	// Currently only support for one gamepad
	static float gamePadAxes[MAX_GAMEPAD_AXES];
	static unsigned char gamePadButtons[MAX_GAMEPAD_BUTTONS];
	static unsigned char lastGamePadButtons[MAX_GAMEPAD_BUTTONS];
};