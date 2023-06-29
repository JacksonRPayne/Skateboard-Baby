#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include "scene/Camera.h"
#include "system/Log.h"
#include "InputManager.h"

#define PIXELS_PER_WORLD_UNIT 128.0f

// Currently only supports one window (static width and height)
class Window
{
public:
	// Must be done before GLAD init
	Window(int width, int height, const char* name);
	// Must be done after GLAD init
	void Init();

	// Getters??
	int GetWidth() { return Window::width; }
	int GetHeight() { return Window::height; }
	static float WorldWidth() { return (float)width / PIXELS_PER_WORLD_UNIT; }
	static float WorldHeight() { return (float)height / PIXELS_PER_WORLD_UNIT; }
	bool IsOpen() { return !glfwWindowShouldClose(window); }
	void SwapBuffers() { glfwSwapBuffers(window); }
	
	static Camera* screenCamera;

	static int width;
	static int height;
private:
	
	// The glfw window instance
	GLFWwindow* window;

	// Displayed at top of window
	const char* name;

};

