#include "SceneManager.h"

// Declare members
std::unordered_map<std::string, Scene*> SceneManager::scenes;
Scene* SceneManager::currentScene = nullptr;
Renderer SceneManager::renderer;

void SceneManager::Init() {
	ResourceManager::LoadShader("res/shaders/default.vert", "res/shaders/default.frag", "default");
	new (&SceneManager::renderer) Renderer(nullptr, ResourceManager::GetShader("default")); // Magic, I love it
}

void SceneManager::Update(float dt) {
	currentScene->Update(dt);
}


void SceneManager::AddScene(Scene* scene) {
	scenes[scene->name] = scene;
	// First scene added
	if (currentScene == nullptr) {
		// Load if needed
		if (!scene->loaded) scene->Load();
		currentScene = scene;
		currentScene->Start();
	}
}

void SceneManager::SetCurrentScene(const std::string& sceneName) {
	// Scene doesn't exist in map
	if (!scenes.count(sceneName)) {
		std::stringstream msg;
		msg << "Scene " << sceneName << " not found.";
		Log::LogError(msg.str());
		return;
	}

	// Pass on projection to next camera (in case of window resize between scenes)
	float aspectRatio = Window::screenCamera->right / WORLD_SCALE;

	if(currentScene) currentScene->End(); // Ends last scene
	currentScene = scenes[sceneName]; // Actual scene change
	// Loads scene if necessary
	if (!currentScene->loaded) {
		currentScene->Load();
	}
	// Screen camera will be swapped in here
	currentScene->Start();
	Window::screenCamera->AdjustProjection(aspectRatio);
}