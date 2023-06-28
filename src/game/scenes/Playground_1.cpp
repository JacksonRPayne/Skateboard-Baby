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
		baby.Render(rend);
		cameraController.followBounds.Render(rend);
		rend->End();
	}
};

Playground_1_Data* sd;
Texture* atlas = nullptr;

void Load_Playground_1() {
	// Load resources
	ResourceManager::LoadTexture("res/textures/Baby.png", "baby");
	atlas = ResourceManager::LoadTexture("res/textures/TextureAtlas.png", "atlas");

	// Initialize data
	sd = new Playground_1_Data();
	new (&sd->camera) Camera(Window::width, Window::height);
	new (&sd->collisionGrid) CollisionGrid(0.5f);
	new (&sd->baby) Baby(0.0f, 0.0f, &sd->collisionGrid);
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
	// Grass 1
	sd->levelRenderer.AddLoopingBackground(atlas, SubTexture(atlas, 0, 9 * 64, 3 * 64, 64), glm::vec2(0.0f, -0.5f), 10, 0.1f);
	// Grass 0
	sd->levelRenderer.AddLoopingBackground(atlas, SubTexture(atlas, 0, 7 * 64, 3 * 64, 64), glm::vec2(0.0f, -0.25f), 10);
	// Path
	sd->levelRenderer.AddLoopingBackground(atlas, SubTexture(atlas, 0, 64 * 4, 64 * 3, 64), glm::vec2(0), 10);
	
}
void Update_Playground_1(float dt){

	sd->Update(dt);
	sd->Render(&SceneManager::renderer);
}
void End_Playground_1() {
	Unload_Playground_1();
}
void Unload_Playground_1() {
	delete sd;
}
