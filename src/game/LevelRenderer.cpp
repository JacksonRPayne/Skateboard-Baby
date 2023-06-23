#include "LevelRenderer.h"


void LevelRenderer::AddParallaxBackground(Texture* tex, SubTexture subTex, glm::vec2 startPos, glm::vec2 scale, CameraController* cameraController, float parallaxIntensity) {
	// Creates transform for parallax background and gives it to the camera controller
	Transform& t = dynamicTileTransforms.emplace_back(startPos, scale, 0.0f);
	cameraController->AddParalaxTarget(&t, parallaxIntensity);
	// Adds renderstep to pipeline
	AddStep([=](Renderer* rend) {
		rend->DrawQuad(tex, subTex, t.position, t.scale);
	});
}

void LevelRenderer::AddLoopingBackground(Texture* tex, SubTexture subTex, glm::vec2 startPos, glm::vec2 scale, Camera* camera, int maxCopies, CameraController* cameraController, float parallaxIntensity) {

	Transform& t = dynamicTileTransforms.emplace_back(startPos, scale, 0.0f);
	// If also a parallax background
	if (cameraController) {
		cameraController->AddParalaxTarget(&t, parallaxIntensity);
	}

	Transform* rootTransform = &t;

	AddStep([=] (Renderer* rend) mutable {
		// Checks if loop needs to be adjusted
		float rightBound = rootTransform->RightBound() + (maxCopies - 1) * rootTransform->scale.x;
		// Needs copies on right side
		if (rightBound < camera->RightBound()) {
			rootTransform->position += glm::vec2(rootTransform->scale.x, 0.0f);
		}
		// Needs copies on left side
		else if (rootTransform->LeftBound() > camera->LeftBound()) {
			rootTransform->position -= glm::vec2(rootTransform->scale.x, 0.0f);
		}

		// Renders the looped tiles
		for (int i = 0; i < maxCopies; i++) {
			rend->DrawQuad(tex, rootTransform->position + glm::vec2(i * rootTransform->scale.x, 0.0f), rootTransform->scale);
		}
	});
}
