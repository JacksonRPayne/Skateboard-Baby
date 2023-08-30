#include "LevelRenderer.h"


void LevelRenderer::AddParallaxBackground(Texture* tex, SubTexture subTex, glm::vec2 startPos, float parallaxIntensity) {
	glm::vec2 scale = tex->WorldSize() * glm::vec2(subTex.coordWidth, subTex.coordHeight);
	AddParallaxBackground(tex, subTex, startPos, scale, parallaxIntensity);
}


void LevelRenderer::AddParallaxBackground(Texture* tex, SubTexture subTex, glm::vec2 startPos, glm::vec2 scale, float parallaxIntensity) {
	// Creates transform for parallax background and gives it to the camera controller
	Transform& t = dynamicTileTransforms.emplace_back(startPos, scale, 0.0f);
	cameraController->AddParalaxTarget(&t, parallaxIntensity);

	Transform* trans = &t;
	// Adds renderstep to pipeline
	AddStep([=](Renderer* rend) {
		rend->DrawQuad(tex, subTex, trans->position, trans->scale);
	});
}

void LevelRenderer::AddLoopingBackground(Texture* tex, SubTexture subTex, glm::vec2 startPos, int maxCopies, float parallaxIntensity) {
	glm::vec2 scale = tex->WorldSize() * glm::vec2(subTex.coordWidth, subTex.coordHeight);
	AddLoopingBackground(tex, subTex, startPos, scale, maxCopies, parallaxIntensity);
}


void LevelRenderer::AddLoopingBackground(Texture* tex, SubTexture subTex, glm::vec2 startPos, glm::vec2 scale, int maxCopies, float parallaxIntensity) {

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
			rend->DrawQuad(tex, subTex, rootTransform->position + glm::vec2(i * (rootTransform->scale.x), 0.0f), rootTransform->scale);
		}

		//scale.x = Window::WorldWidth();
		//rend->DrawQuad(tex, subTex, rootTransform->position, scale, true);
	});
}
