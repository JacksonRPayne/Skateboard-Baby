#include "Scene_Testing.h"

void RenderLevelTiles(Renderer* rend);

struct SceneTestingData {
	SceneTestingData() : atlas(nullptr), rail(nullptr), ground(nullptr) {};
	
	Baby player;
	Camera camera;
	CameraController camController;
	Texture* atlas;
	HitBox* rail;
	HitBox* ground;
	Transform t = Transform(0.0f, 0.35f, 0.5f, 0.5f, 0.0f);
	SubTexture s = SubTexture();
	CollisionGrid grid;
	LevelRenderer levelRenderer;

	void Update(float dt) {
		player.Update(dt);
		camController.Update(dt);
	}
	void Render(Renderer* rend) {
		rend->Start();
		levelRenderer.Render(rend);
		RenderLevelTiles(rend);
		player.Render(rend);
		//rail->Render(rend);
		//ground->Render(rend);
		// grid.DEBUG_RENDER(rend);
		//camController.followBounds.Render(rend);
		rend->End();
	}
};

// sd for "scene data", needs a short name bc its used a lot
SceneTestingData* sd = nullptr; 


void RenderLevelTiles(Renderer* rend) {
	//Texture* t = ResourceManager::GetTexture("backdrop");
	//rend->DrawQuad(t, glm::vec2(0.0f, 0.0f), glm::vec2(t->width / 128.0f, t->height / 128.0f));
	// Testing background
	//sd->TEST_BACKGROUND.Render(rend);

	// Draw Ground
	sd->t.SetPosition(0.0f, 0.35f);
	sd->s.SetValues(sd->atlas, 0, 0, 64, 64);
	rend->DrawQuad(sd->atlas, sd->s, sd->t.GetPosition(), sd->t.GetScale());
	sd->t.Translate(0.5f, 0.0f);
	sd->s.SetValues(sd->atlas, 64, 0, 64, 64);
	rend->DrawQuad(sd->atlas, sd->s, sd->t.GetPosition(), sd->t.GetScale());
	for (int i = 0; i < 50; i++) {
		sd->t.Translate(0.5f, 0.0f);
		rend->DrawQuad(sd->atlas, sd->s, sd->t.GetPosition(), sd->t.GetScale());
	}
	sd->t.Translate(0.5f, 0.0f);
	sd->s.SetValues(sd->atlas, 128, 0, 64, 64);
	rend->DrawQuad(sd->atlas, sd->s, sd->t.GetPosition(), sd->t.GetScale());

	// Draw Rail
	sd->t.SetPosition(2.0, 0.25);
	sd->s.SetValues(sd->atlas, 0, 128, 64, 64);
	rend->DrawQuad(sd->atlas, sd->s, sd->t.GetPosition(), sd->t.GetScale());
	sd->s.SetValues(sd->atlas, 64, 128, 64, 64);
	for (int i = 0; i < 20; i++) {
		sd->t.Translate(0.5f, 0.0f);
		rend->DrawQuad(sd->atlas, sd->s, sd->t.GetPosition(), sd->t.GetScale());
	}
	sd->t.Translate(0.5f, 0.0f);
	sd->s.SetValues(sd->atlas, 2 * 64, 128, 64, 64);
	rend->DrawQuad(sd->atlas, sd->s, sd->t.GetPosition(), sd->t.GetScale());

}

void Load_Testing(){
	sd = new SceneTestingData();

	ResourceManager::LoadTexture("res/textures/Baby.png", "baby");
	ResourceManager::LoadTexture("res/textures/Grid.png", "testgrid", false);
	ResourceManager::LoadTexture("res/textures/Background.png", "backdrop");
	sd->atlas = ResourceManager::LoadTexture("res/textures/TextureAtlas.png", "atlas");

	// Populate scene data
	new (&sd->camera) Camera(Window::width, Window::height);
	// I do these up here so cam controller can set up automatically
	sd->camera.transform.ScaleFactor(2.0f, 2.0f);
	sd->camera.transform.Translate(3.3f, -1.4f);

	new (&sd->grid) CollisionGrid(0.5f);
	new (&sd->player) Baby(0.0f, 0.0f, 1.0f, 1.0f, 0.0f, &sd->grid); // <-- more goated than std::move
	new (&sd->camController) CameraController(&sd->camera);

	sd->rail = sd->grid.Register(HitBox(2.0, 2.0 + 21.0f * 0.5f, -0.125f, 0.125f, HitBoxType::GrindRail));
	sd->ground = sd->grid.Register(HitBox(-0.25f, 51.0f*0.5f + 0.75f, 0.5f, 1.0f, HitBoxType::Ground));
	sd->grid.ConstructGrid();
}

void Start_Testing() {
	// Set our camera as the used one
	Window::screenCamera = &sd->camera;
	SceneManager::renderer.camera = &sd->camera;

	sd->camController.SetFollowTarget(&sd->player.transform, -1.5f, 0.2f, -0.5f, 2.0f);
	//sd->camController.AddParalaxTarget(&sd->TEST_BACKGROUND.rootTransform, 0.8f);
	sd->levelRenderer.AddLoopingBackground(ResourceManager::GetTexture("testgrid"), SubTexture(), 
		glm::vec2(0.0f, 0.0f), glm::vec2(2.0f, 2.0f), &sd->camera, 5, &sd->camController, 0.8f);
}

void Update_Testing(float dt) {

	
	if (InputManager::GetKey(GLFW_KEY_RIGHT)) {
		sd->camera.transform.Translate(4.0f * dt, 0.0f);
	}
	if (InputManager::GetKey(GLFW_KEY_LEFT)) {
		sd->camera.transform.Translate(-4.0f * dt, 0.0f);
	}
	if (InputManager::GetKey(GLFW_KEY_UP)) {
		sd->camera.transform.Translate(0.0, -4.0f * dt);
	}
	if (InputManager::GetKey(GLFW_KEY_DOWN)) {
		sd->camera.transform.Translate(0.0f, 4.0f * dt);
	}
	if (InputManager::GetKey(GLFW_KEY_RIGHT_SHIFT)) {
		sd->camera.transform.Scale(-4.0f * dt, -4.0f * dt);
	}
	if (InputManager::GetKey(GLFW_KEY_RIGHT_CONTROL)) {
		sd->camera.transform.Scale(4.0f * dt, 4.0f * dt);
	}
	
	sd->Update(dt);
	sd->Render(&SceneManager::renderer);
	
	glm::vec2 mPos = InputManager::GetWorldMousePos(Window::width, Window::height, sd->camera.right, sd->camera.transform)- sd->camera.transform.position;
	//std::cout << mPos.x << ", " << mPos.y << "\n";
	//std::cout << Window::width << ", " << Window::height << '\n';
}

void End_Testing() {

}

void Unload_Testing() {
	delete sd;
}