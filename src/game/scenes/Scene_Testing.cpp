#include "Scene_Testing.h"

struct Scene_Testing_Data {
	// Scene
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

		//collisionGrid.DEBUG_RENDER(rend);

	}

};

// sd for "scene data", needs a short name bc its used a lot
Scene_Testing_Data* sd = nullptr;


void Load_Testing(){
	// Load resources
	ResourceManager::LoadTexture("res/textures/Baby.png", "baby");
	ResourceManager::LoadTexture("res/textures/Background.png", "background");
	ResourceManager::LoadTexture("res/textures/Tileset.png", "tileset");

	// Init data
	sd = new Scene_Testing_Data();
	new (&sd->camera) Camera(Window::width, Window::height);
	new (&sd->collisionGrid) CollisionGrid(0.5f);
	new (&sd->baby) Baby(0.0f, 1.25f, &sd->collisionGrid);
	new (&sd->cameraController) CameraController(&sd->camera);
	new (&sd->levelRenderer) LevelRenderer(&sd->camera, &sd->cameraController);

}

void Start_Testing() {
	// Set our camera as main camera
	SceneManager::SetCamera(&sd->camera);
	sd->camera.transform.scale = glm::vec2(2.0f, 2.0f);

	// Set ground collision
	sd->collisionGrid.Register(HitBox(-0.5f, 9.25f, 1.75f, 2.0f, HitBoxType::Ground));
	sd->collisionGrid.Register(HitBox(9.25f, 9.75f, 1.25f, 1.75f, HitBoxType::Ramp));
	sd->collisionGrid.Register(HitBox(9.75f, 20.0f, 1.25f, 1.75f, HitBoxType::Ground));

	// Set up our scene objects
	sd->collisionGrid.ConstructGrid();
	sd->cameraController.SetFollowTarget(&sd->baby.transform, -1.5f, 0.2f, -0.5f, 2.0f);

	// -- LEVEL RENDERER --
	float levelY = 1.25f;
	// Easy access to tileset
	Texture* atlas = ResourceManager::GetTexture("tileset");
	// Backdrop
	sd->levelRenderer.AddParallaxBackground(ResourceManager::GetTexture("background"), SubTexture(), glm::vec2(0), 1.0f);
	// Bushes
	sd->levelRenderer.AddLoopingBackground(atlas, SubTexture(atlas, 6 * 64, 0, 2 * 64, 3 * 64), glm::vec2(0.0f, levelY - 0.1f), 10, 0.1f);
	// Ground
	//sd->levelRenderer.AddLoopingBackground(atlas, SubTexture(atlas, 1 * 64, 1*64, 2 * 64, 1 * 64), glm::vec2(0.0f, levelY+0.5f ), 10, 0.0f);
	sd->levelRenderer.AddStep([=](Renderer* rend) {
		SubTexture st = SubTexture(atlas, 1 * 64, 1 * 64, 2 * 64, 1 * 64);
		Transform t = Transform(glm::vec2(0.0f, levelY + 0.5f), glm::vec2(1.0f, 0.5f), 0.0f);
		for (int i = 0; i < 10; i++) {
			rend->DrawQuad(atlas, st, t.position, t.scale);
			t.Translate(1.0f, 0.0f);
		}
		t.Translate(-0.5f, -0.25f);
		rend->DrawQuad(atlas, SubTexture(atlas, 3 * 64, 0 * 64, 1 * 64, 2 * 64), t.position, glm::vec2(0.5f, 1.0f));
		t.Translate(0.75f, -0.25f);
		for (int i = 0; i < 10; i++) {
			rend->DrawQuad(atlas, st, t.position, t.scale);
			t.Translate(1.0f, 0.0f);
		}
		});
	// Baby
	sd->levelRenderer.AddStep([](Renderer* rend) {sd->baby.Render(rend); });


}

void Update_Testing(float dt) {
	sd->Update(dt);
	// TODO: u should store this stuff (renderer and cam) in Scene as static vars
	sd->Render(&SceneManager::renderer);

	if (InputManager::GetKey(GLFW_KEY_RIGHT_SHIFT)) {
		sd->camera.transform.Scale(-4.0f * dt, -4.0f * dt);
	}
	if (InputManager::GetKey(GLFW_KEY_RIGHT_CONTROL)) {
		sd->camera.transform.Scale(4.0f * dt, 4.0f * dt);
	}

	if (InputManager::GetKeyDown(GLFW_KEY_R)) {
		End_Testing();
		Load_Testing();
		Start_Testing();
	}
}

void End_Testing() {
	Unload_Testing();
}

void Unload_Testing() {
	delete sd;
}