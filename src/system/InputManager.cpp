#include "InputManager.h"

bool InputManager::keys[NUM_KEYS] = { 0 };
bool InputManager::lastKeys[NUM_KEYS] = { 0 };
float InputManager::gamePadAxes[MAX_GAMEPAD_AXES] = { 0.0f };
unsigned char InputManager::gamePadButtons[MAX_GAMEPAD_BUTTONS] = { 0 };
unsigned char InputManager::lastGamePadButtons[MAX_GAMEPAD_BUTTONS] = { 0 };

bool InputManager::mouseButtons[NUM_MOUSE_BUTTONS] = { 0 };
bool InputManager::lastMouseButtons[NUM_MOUSE_BUTTONS] = { 0 };

double InputManager::mouseX = 0;
double InputManager::mouseY = 0;

bool InputManager::GetKey(int key) {
	return keys[key];
}

bool InputManager::GetKeyDown(int key) {
	return keys[key] && !lastKeys[key];
}

bool InputManager::GetKeyUp(int key) {
	return !keys[key] && lastKeys[key];
}

bool InputManager::GetMouseButton(int button) {
	return mouseButtons[button];
}

bool InputManager::GetMouseButtonDown(int button) {
	return mouseButtons[button] && !lastMouseButtons[button];
}

bool InputManager::GetMouseButtonUp(int button){
	return !mouseButtons[button] && lastMouseButtons[button];
}

float InputManager::GetGamepadAxisRaw(int axis) {
	return gamePadAxes[axis];
}
float InputManager::GetGamepadAxisDigital(int axis) {
	if (std::abs(gamePadAxes[axis]) > GAMEPAD_DEAD_ZONE) {
		return gamePadAxes[axis] > 0 ? 1.0f : -1.0f;
	}

	return 0;
}

bool InputManager::GetGamepadButton(int button) {
	return gamePadButtons[button] == GLFW_PRESS;
}

bool InputManager::GetGamepadButtonDown(int button) {
	return gamePadButtons[button] == GLFW_PRESS && !(lastGamePadButtons[button] == GLFW_PRESS);
}

bool InputManager::GetGamepadButtonUp(int button) {
	return !(gamePadButtons[button] == GLFW_PRESS) && (lastGamePadButtons[button] == GLFW_PRESS);
}

glm::vec2 InputManager::GetWorldMousePos(int windowWidth, int windowHeight, float camRight, const Transform &camTransform) {
	// TODO: is this a matrix operation? probably
	// TODO: also change input to be camera pointer?
	float xNorm = ((mouseX / windowWidth)*2 - 1.0f)*camTransform.GetScale().x;
	float yNorm = ((mouseY / windowHeight)*2 - 1.0f )* camTransform.GetScale().y;;
	return  glm::vec2(xNorm * camRight + camTransform.GetPosition().x, yNorm + camTransform.GetPosition().y);
}

void InputManager::Update() {
	std::copy(keys, keys + NUM_KEYS, lastKeys);
	std::copy(mouseButtons, mouseButtons+ NUM_MOUSE_BUTTONS, lastMouseButtons);


	// If a gamepad is connected (currently only works with one gamepad)
	if (glfwJoystickPresent(GLFW_JOYSTICK_1) && glfwJoystickIsGamepad(GLFW_JOYSTICK_1)) {
		// Get the analog axes
		int axesCount;
		const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);
		// Clamps axes to max
		axesCount = std::min(axesCount, MAX_GAMEPAD_AXES);
		memcpy(gamePadAxes, axes, sizeof(float)*axesCount);

		// Get the joystick buttons
		int buttonCount;
		const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);
		// Clamps buttons
		buttonCount = std::min(buttonCount, MAX_GAMEPAD_BUTTONS);
		// Retains button presses from last frame
		memcpy(lastGamePadButtons, gamePadButtons, MAX_GAMEPAD_BUTTONS);
		memcpy(gamePadButtons, buttons, buttonCount);
	}

}