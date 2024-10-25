#include "Scene_Testing.h"

struct Scene_Testing_Data {
	// Scene
	Camera camera;
	CameraController cameraController;
	LevelRenderer levelRenderer;
	LevelBuilder levelBuilder;
	CollisionGrid collisionGrid;

	// Entities
	Baby baby;
	Bully bully1;

	void Update(float dt) {
		baby.Update(dt);
		bully1.Update(dt);
		cameraController.Update(dt);
	}

	void Render(Renderer* rend) {
		rend->Start();
		levelRenderer.Render(rend);
		rend->End();

		if(InputManager::GetKey(GLFW_KEY_0)||InputManager::GetGamepadButton(GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER)) collisionGrid.DEBUG_RENDER(rend);
		//cameraController.followBounds.Render(rend);
	}

};

// sd for "scene data", needs a short name bc its used a lot
Scene_Testing_Data* sd = nullptr;


void Load_Testing(){
	// Load resources
	ResourceManager::LoadTexture("res/textures/Baby.png", "baby");
	ResourceManager::LoadTexture("res/textures/Bully.png", "bully");
	ResourceManager::LoadTexture("res/textures/Background.png", "background");
	ResourceManager::LoadTexture("res/textures/Tileset.png", "tileset");

	// Init data
	sd = new Scene_Testing_Data();
	new (&sd->camera) Camera(Window::width, Window::height);
	new (&sd->collisionGrid) CollisionGrid(0.5f);
	new (&sd->baby) Baby(-2.0f, 1.25f, &sd->collisionGrid);
	new (&sd->bully1) Bully(6.0f, 1.25f, &sd->collisionGrid);
	new (&sd->cameraController) CameraController(&sd->camera);
	new (&sd->levelRenderer) LevelRenderer(&sd->camera, &sd->cameraController);
	new (&sd->levelBuilder) LevelBuilder(glm::vec2(-4.0f, 1.95f), &sd->levelRenderer, &sd->collisionGrid, ResourceManager::GetTexture("tileset"));
}

void Start_Testing() {
	// Set our camera as main camera
	SceneManager::SetCamera(&sd->camera);
	sd->camera.transform.scale = glm::vec2(2.0f, 2.0f);

	// -- LEVEL RENDERER --
	float levelY = 1.25f;
	// Easy access to tileset
	Texture* atlas = ResourceManager::GetTexture("tileset");
	// Backdrop
	sd->levelRenderer.AddParallaxBackground(ResourceManager::GetTexture("background"), SubTexture(), glm::vec2(0), 1.0f);
	// Bushes
	sd->levelRenderer.AddLoopingBackground(atlas, SubTexture(atlas, 6 * 64, 0, 2 * 64, 3 * 64), glm::vec2(0.0f, levelY - 0.1f), 10, 0.1f);
	// Ground
	sd->levelBuilder.AddGround(SubTexture(atlas, 64, 64, 64, 64), 10);
	sd->levelBuilder.AddUpRampCliff(SubTexture(atlas, 64 * 3, 0, 64, 128), 2);
	sd->levelBuilder.AddGround(SubTexture(atlas, 64, 64, 64, 64), 18);
	sd->levelBuilder.AddUpRamp(SubTexture(atlas, 64 * 3, 0, 64, 128), 11);
	sd->levelBuilder.AddGround(SubTexture(atlas, 64, 64, 64, 64), 5);
	sd->levelBuilder.AddDownRamp(SubTexture(atlas, 64 * 4, 0, 64, 128), 11);
	
	sd->levelBuilder.AddGround(SubTexture(atlas, 64, 64, 64, 64), 15);
	sd->levelBuilder.AddUpRamp(SubTexture(atlas, 64 * 3, 0, 64, 128), 11);
	sd->levelBuilder.AddGround(SubTexture(atlas, 64, 64, 64, 64), 5);
	sd->levelBuilder.AddDownRamp(SubTexture(atlas, 64 * 4, 0, 64, 128), 11);
	
	sd->levelBuilder.AddGround(SubTexture(atlas, 64, 64, 64, 64), 15);
	sd->levelBuilder.AddUpRamp(SubTexture(atlas, 64 * 3, 0, 64, 128), 11);
	sd->levelBuilder.AddGround(SubTexture(atlas, 64, 64, 64, 64), 5);
	sd->levelBuilder.AddDownRamp(SubTexture(atlas, 64 * 4, 0, 64, 128), 11);
	sd->levelBuilder.AddGround(SubTexture(atlas, 64, 64, 64, 64), 15);
	
	sd->levelBuilder.Build();

	// Baby
	sd->levelRenderer.AddStep([](Renderer* rend) {sd->baby.Render(rend); });

	// Bully
	sd->levelRenderer.AddStep([](Renderer* rend) {sd->bully1.Render(rend); });

	// Testing wall
	sd->levelRenderer.AddStep([=](Renderer* rend) {
		rend->DrawQuad(atlas, SubTexture(atlas, 64, 3 * 64, 64, 64 * 3), glm::vec2(3.0f, 1.0f), glm::vec2(0.5f, 1.5f));
		});

	// Testing grind rail
	sd->levelRenderer.AddStep([=](Renderer* rend) {
		for (int i = 0; i < 4; i++) {
			glm::vec2 pos(3.5f + i * 0.5, 0.5f);
			rend->DrawQuad(atlas, SubTexture(atlas, 2 * 64, 3 * 64, 64, 64), pos, glm::vec2(0.5f, 0.5f));
		}
		});

	// Testing wall2
	sd->levelRenderer.AddStep([=](Renderer* rend) {
		rend->DrawQuad(atlas, SubTexture(atlas, 64*3, 3 * 64, 64, 64 * 3), glm::vec2(3.0f+2.5f, 1.0f), glm::vec2(0.5f, 1.5f));
		});


	// Testing wall 1hitbox
	sd->collisionGrid.Register(HitBox(3.0f, 1.1f, 0.15f, 1.3f, nullptr, nullptr, nullptr, HitBoxType::Wall));
	// Testing grind rail hitbox
	sd->collisionGrid.Register(HitBox(3.25f + 1.0f, 0.125f, 2.5f, 0.5f, nullptr, nullptr, nullptr, HitBoxType::GrindRail));
	// Testing wall 2 hitbox
	sd->collisionGrid.Register(HitBox(3.0f + 2.5f,1.1f, 0.15f, 1.3f, nullptr, nullptr, nullptr, HitBoxType::Wall));

	// Set up our scene objects
	sd->collisionGrid.ConstructGrid();
	sd->cameraController.SetFollowTarget(&sd->baby.transform, -1.5f, 0.2f, -0.5f, 1.5f);

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

	if (InputManager::GetKeyDown(GLFW_KEY_R)|| InputManager::GetGamepadButtonDown(GLFW_GAMEPAD_BUTTON_START)) {
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