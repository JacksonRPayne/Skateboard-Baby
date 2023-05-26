#include "Scene_Testing.h"

void RenderLevelTiles(Renderer* rend);

struct SceneTestingData {
	SceneTestingData() : atlas(nullptr) {};
	
	Baby player;
	Camera camera;
	Texture* atlas;
	Transform t = Transform(0.0f, 0.35f, 0.5f, 0.5f, 0.0f);
	SubTexture s = SubTexture();
	HitBox rail;

	void Update(float dt) {
		player.Update(dt);
	}
	void Render(Renderer* rend) {
		rend->Start();
		RenderLevelTiles(rend);
		player.Render(rend);
		rend->End();
	}
};

SceneTestingData sd; // sd for "scene data", needs a short name bc its used a lot


void RenderLevelTiles(Renderer* rend) {
	// This whole section (apart from player render) is still slow btw 
	sd.t.SetPosition(0.0f, 0.35f);
	sd.s.SetValues(sd.atlas, 0, 0, 64, 64);
	rend->DrawQuad(sd.atlas, sd.s, sd.t.GetModelMatrix()); // TODO: implement w/o needing matrix
	sd.t.Translate(0.5f, 0.0f);
	sd.s.SetValues(sd.atlas, 64, 0, 64, 64);
	rend->DrawQuad(sd.atlas, sd.s, sd.t.GetModelMatrix());
	for (int i = 0; i < 50; i++) {
		sd.t.Translate(0.5f, 0.0f);
		rend->DrawQuad(sd.atlas, sd.s, sd.t.GetModelMatrix());
	}
	sd.t.Translate(0.5f, 0.0f);
	sd.s.SetValues(sd.atlas, 128, 0, 64, 64);
	rend->DrawQuad(sd.atlas, sd.s, sd.t.GetModelMatrix());

	sd.t.SetPosition(2.0, 0.25);
	sd.s.SetValues(sd.atlas, 0, 128, 64, 64);
	rend->DrawQuad(sd.atlas, sd.s, sd.t.GetModelMatrix());
	sd.s.SetValues(sd.atlas, 64, 128, 64, 64);
	for (int i = 0; i < 20; i++) {
		sd.t.Translate(0.5f, 0.0f);
		rend->DrawQuad(sd.atlas, sd.s, sd.t.GetModelMatrix());
	}
	sd.t.Translate(0.5f, 0.0f);
	sd.s.SetValues(sd.atlas, 2 * 64, 128, 64, 64);
	rend->DrawQuad(sd.atlas, sd.s, sd.t.GetModelMatrix());

	//sd.rail.Render(rend);
}

void Load_Testing(){
	ResourceManager::LoadTexture("res/textures/Baby.png", "baby");
	sd.atlas = ResourceManager::LoadTexture("res/textures/TextureAtlas.png", "atlas");

	// Populate scene data
	//sd.player = std::move(Baby(0.0f, 0.0f, 1.0f, 1.0f, 0.0f));
	new (&sd.player) Baby(0.0f, 0.0f, 1.0f, 1.0f, 0.0f); // <-- more goated than std::move
	new (&sd.camera) Camera(Window::width, Window::height);
	new (&sd.rail) HitBox(2.0, 25.0f * 0.5f, -0.25f, 0.125f, HitBoxType::GrindRail); // TODO: this calculation doesn't feel right...
}

void Start_Testing() {
	// Set our camera as the used one
	Window::screenCamera = &sd.camera;
	SceneManager::renderer.camera = &sd.camera;

	sd.camera.transform.ScaleFactor(2.0f, 2.0f);
	sd.camera.transform.Translate(1.5f, -1.4f);
}

void Update_Testing(float dt) {

	
	if (InputManager::GetKey(GLFW_KEY_RIGHT)) {
		sd.camera.transform.Translate(4.0f * dt, 0.0f);
	}
	if (InputManager::GetKey(GLFW_KEY_LEFT)) {
		sd.camera.transform.Translate(-4.0f * dt, 0.0f);
	}
	if (InputManager::GetKey(GLFW_KEY_UP)) {
		sd.camera.transform.Translate(0.0, -4.0f * dt);
	}
	if (InputManager::GetKey(GLFW_KEY_DOWN)) {
		sd.camera.transform.Translate(0.0f, 4.0f * dt);
	}
	if (InputManager::GetKey(GLFW_KEY_RIGHT_SHIFT)) {
		sd.camera.transform.Scale(-4.0f * dt, -4.0f * dt);
	}
	if (InputManager::GetKey(GLFW_KEY_RIGHT_CONTROL)) {
		sd.camera.transform.Scale(4.0f * dt, 4.0f * dt);
	}

	// Brute force collision check for now
	sd.player.boardHitBox.CheckCollision(sd.rail);
	sd.Update(dt);
	// This is so far from what I was trying to do haha
	float playerX = sd.player.transform.GetPosition().x;
	float rightBound = sd.camera.right * sd.camera.transform.GetScale().x - 0.8f;
	float leftBound = sd.camera.left * sd.camera.transform.GetScale().x + 0.8f;
	if(playerX >= rightBound || playerX <= leftBound) sd.camera.transform.SetPositionX(sd.player.transform.GetPosition().x);
	sd.Render(&SceneManager::renderer);

}

void End_Testing() {

}

void Unload_Testing() {

}