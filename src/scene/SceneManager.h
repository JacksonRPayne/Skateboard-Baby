#pragma once
#include <map>
#include <unordered_map>
#include <string>
#include <sstream>
#include "Camera.h"
#include "Scene.h"
#include "system/Window.h"


class SceneManager
{
public:
	// Adds needed scenes and sets up renderer
	static void Init();
	static Scene* GetCurrentScene() { return currentScene; }

	// Scenes keep state between switches (maybe create scene reset method?)
	static void SetCurrentScene(const std::string &sceneName);
	static void SetCamera(Camera* camera);
	static void AddScene(Scene* scene);

	static void Update(float dt);

	static std::unordered_map<std::string, Scene*> scenes;
	static Renderer renderer;

private:

	static Scene* currentScene;
};

