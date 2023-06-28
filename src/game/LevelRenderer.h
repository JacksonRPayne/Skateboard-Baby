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
// NOTE 2: If this ever becomes a preformanc issue because of std::function and lambdas alone, you could convert to "c" style
// -- by creating a vector of data structs that hold data for each step function and pass an index into the step function.
// -- i thiiiiiiiiiiiink that would work but hopefully you don't have to do anything like that.
struct LevelRenderer {

	LevelRenderer() :renderSteps(){}
	LevelRenderer(Camera* cam, CameraController* camController) : camera(cam), cameraController(camController), renderSteps(){}

	void Render(Renderer* rend) {
		for (int i = 0; i < renderSteps.size(); i++) {
			renderSteps[i](rend);
		}
	}
	// Adds render step to renderer
	void AddStep(const std::function<void(Renderer*)>& step) { renderSteps.push_back(step); }
	// For adding specific types of elements to level
	void AddParallaxBackground(Texture* tex, SubTexture subTex, glm::vec2 startPos, float parallaxIntensity); // Determines scale from tex
	void AddParallaxBackground(Texture* tex, SubTexture subTex, glm::vec2 startPos, glm::vec2 scale, float parallaxIntensity);
	void AddLoopingBackground(Texture* tex, SubTexture subTex, glm::vec2 startPos, int maxCopies, float parallaxIntensity = 0.0f);
	void AddLoopingBackground(Texture* tex, SubTexture subTex, glm::vec2 startPos, glm::vec2 scale, int maxCopies, float parallaxIntensity=0.0f);

	// List of steps
	std::vector<std::function<void(Renderer*)>> renderSteps;
	// For tiles that have dynamic positions, need to keep track of them 
	std::list<Transform> dynamicTileTransforms;

	Camera* camera = nullptr;
	CameraController* cameraController = nullptr;
};


