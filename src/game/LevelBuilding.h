#pragma once
#include "graphics/Transform.h"
#include "scene/Camera.h"
#include "graphics/Texture.h"
#include "graphics/Renderer.h"

// For a background texture that should be looped on the x axis if it leaves the camera's sight
struct LoopingTexture {
	LoopingTexture() :texture(nullptr), camera(nullptr){}
	LoopingTexture(Texture* texture, glm::vec2 startPos, glm::vec2 scale, Camera* camera, int maxCopies);
	LoopingTexture(Texture* texture, SubTexture subTex, glm::vec2 startPos, glm::vec2 scale, Camera* camera, int maxCopies);

	// The transform of the leftmost texture
	Transform rootTransform;
	Camera* camera;
	Texture* texture;
	SubTexture subTexture;
	int maxCopies;

	void Update();
	void Render(Renderer* rend);
};