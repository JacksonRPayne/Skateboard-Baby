#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include "system/Window.h"
#include "system/InputManager.h"
#include "system/Log.h"
#include "scene/SceneManager.h"
#include "game/scenes/Scenes.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

class Game
{
public:
	Game(const std::string &name);
	int Run();

private:
	std::string name;
};

