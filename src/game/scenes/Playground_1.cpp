#include "Playground_1.h"

struct Playground_1_Data {
	// Level stuff
	Camera camera;
	CameraController cameraController;
	LevelRenderer levelRenderer;
	CollisionGrid collisionGrid;
	
	// Entities
	Baby baby;

	void Update(float dt) {
		baby.Update(dt);
		cameraController.Update(dt);
	}
	void Render(Renderer* rend) {
		rend->Start();
		levelRenderer.Render(rend);
		rend->End();
	}
};

Playground_1_Data* sd;
Texture* atlas = nullptr;

void Load_Playground_1() {
	// Load resources
	ResourceManager::LoadTexture("res/textures/Baby.png", "baby");
	ResourceManager::LoadTexture("res/textures/Background.png", "background");
	atlas = ResourceManager::LoadTexture("res/textures/TextureAtlas.png", "atlas");

	// Initialize data
	sd = new Playground_1_Data();
	new (&sd->camera) Camera(Window::width, Window::height);
	new (&sd->collisionGrid) CollisionGrid(0.5f);
	new (&sd->baby) Baby(0.0f, 1.05f, &sd->collisionGrid);
	new (&sd->cameraController) CameraController(&sd->camera);
	new (&sd->levelRenderer) LevelRenderer(&sd->camera, &sd->cameraController);

	// Etc
	sd->collisionGrid.ConstructGrid();
	
}
void Start_Playground_1() {
	// TODO: is there a way that I don't have to always do this?
	// -- make camera singletonesque??
	SceneManager::SetCamera(&sd->camera);
	sd->camera.transform.scale = glm::vec2(2.0f, 2.0f);

	// Set up camera controller
	sd->cameraController.SetFollowTarget(&sd->baby.transform, -1.5f, 0.2f, -0.5f, 2.0f);

	// -- LEVEL RENDER STEPS --
	float levelYOffset = 1.4f;
	// Backdrop
	sd->levelRenderer.AddParallaxBackground(ResourceManager::GetTexture("background"), SubTexture(), glm::vec2(0), 1.0f);
	//Houses
	sd->levelRenderer.AddLoopingBackground(atlas, SubTexture(atlas, 5 * 64, 6 * 64, 2* 64, 2* 64), glm::vec2(0.0f, levelYOffset - 1.55f), 20, 0.45f);
	// Street 1
	sd->levelRenderer.AddLoopingBackground(atlas, SubTexture(atlas, 7 * 64, 3 * 64, 64, 64), glm::vec2(0.0f, levelYOffset - 1.1f), 20, 0.45f);
	// Street 0
	sd->levelRenderer.AddLoopingBackground(atlas, SubTexture(atlas, 7 * 64, 4 * 64, 64, 64), glm::vec2(0.0f, levelYOffset - 0.62f), 20, 0.3f);
	// Fence
	sd->levelRenderer.AddLoopingBackground(atlas, SubTexture(atlas, 5*64, 3 * 64, 64, 2 * 64), glm::vec2(0.0f, levelYOffset - 1.1f), 20, 0.2f);
	// Grass 2
	sd->levelRenderer.AddLoopingBackground(atlas, SubTexture(atlas, 64, 11 * 64, 2 * 64, 64), glm::vec2(0.0f, levelYOffset - 0.65f), 10, 0.2f);
	// Trees 1
	sd->levelRenderer.AddLoopingBackground(atlas, SubTexture(atlas, 9*64, 0, 4 * 64, 6*64), glm::vec2(0.0f, levelYOffset - 1.7f), 10, 0.15f);
	// Grass 1
	sd->levelRenderer.AddLoopingBackground(atlas, SubTexture(atlas, 64, 9 * 64, 3 * 64, 64), glm::vec2(0.0f, levelYOffset-0.52f), 10, 0.1f);
	// Trees 0
	sd->levelRenderer.AddLoopingBackground(atlas, SubTexture(atlas, 9 * 64, 0, 4 * 64, 6 * 64), glm::vec2(0.7f, levelYOffset - 1.5f), 10, 0.05f);
	// Grass 0
	sd->levelRenderer.AddLoopingBackground(atlas, SubTexture(atlas, 64, 7 * 64, 3 * 64, 64), glm::vec2(0.0f, levelYOffset-0.25f), 10);
	// Path
	sd->levelRenderer.AddLoopingBackground(atlas, SubTexture(atlas, 64, 64 * 4, 64 * 3, 64), glm::vec2(0, levelYOffset), 10);
	// Baby
	sd->levelRenderer.AddStep([](Renderer* rend) { sd->baby.Render(rend); });
	// Grass -1
	sd->levelRenderer.AddLoopingBackground(atlas, SubTexture(atlas, 64*4, 0, 64 * 4, 64*2), glm::vec2(0, levelYOffset+0.2f), 10, -0.2f);
	// Shadows 1
	//sd->levelRenderer.AddLoopingBackground(atlas, SubTexture(atlas, 10 * 64, 6*64, 4 * 64, 3 * 64), glm::vec2(0.6f, levelYOffset + 0.1f), 10, 0.15f);
	// Shadows 0 
	//sd->levelRenderer.AddLoopingBackground(atlas, SubTexture(atlas, 10 * 64, 6 * 64, 4 * 64, 3 * 64), glm::vec2(0.6f+0.7f, levelYOffset + 0.3f), 10, 0.05f);
}


void Update_Playground_1(float dt){
	if (InputManager::GetKey(GLFW_KEY_RIGHT_SHIFT)) {
		sd->camera.transform.Scale(-4.0f * dt, -4.0f * dt);
	}
	if (InputManager::GetKey(GLFW_KEY_RIGHT_CONTROL)) {
		sd->camera.transform.Scale(4.0f * dt, 4.0f * dt);
	}
	sd->Update(dt);
	sd->Render(&SceneManager::renderer);
}
void End_Playground_1() {
	Unload_Playground_1();
}
void Unload_Playground_1() {
	delete sd;
}
