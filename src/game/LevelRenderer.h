#pragma once
#include <vector>
#include <list>
#include <functional>
#include "graphics/Transform.h"
#include "scene/Camera.h"
#include "graphics/Texture.h"
#include "graphics/Renderer.h"
#include "game/CameraController.h"


// Functional approach to rendering a level, sort of like turtle graphics
// Data of level isn't stored anywhere, only stores a vector of function pointers that it calls one by one to render the level.
// NOTE: wow I can't believe this actually works
struct LevelRenderer {

	LevelRenderer() :renderSteps(){}

	void Render(Renderer* rend) {
		for (int i = 0; i < renderSteps.size(); i++) {
			renderSteps[i](rend);
		}
	}
	// Adds render step to renderer
	void AddStep(const std::function<void(Renderer*)>& step) { renderSteps.push_back(step); }
	// For adding specific types of elements to level
	void AddParallaxBackground(Texture* tex, SubTexture subTex, glm::vec2 startPos, glm::vec2 scale, CameraController* cameraController, float parallaxIntensity);
	void AddLoopingBackground(Texture* tex, SubTexture subTex, glm::vec2 startPos, glm::vec2 scale, Camera* camera, int maxCopies, CameraController* cameraController=nullptr, float parallaxIntensity=0.0f);

	// List of steps
	std::vector<std::function<void(Renderer*)>> renderSteps;
	// For tiles that have dynamic positions, need to keep track of them 
	std::list<Transform> dynamicTileTransforms;
};


