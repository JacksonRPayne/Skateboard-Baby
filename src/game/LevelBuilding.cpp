#include "LevelBuilding.h"

LoopingBackground::LoopingBackground(Texture* texture, glm::vec2 startPos, glm::vec2 scale, Camera* camera, int maxCopies) 
	: rootTransform(startPos, scale, 0.0f), camera(camera), texture(texture), maxCopies(maxCopies){}

void LoopingBackground::Update() {
	float rightBound = rootTransform.RightBound() + (maxCopies - 1) * rootTransform.scale.x;
	// Needs copies on right side
	if (rightBound < camera->RightBound()) {
		rootTransform.position += glm::vec2(rootTransform.scale.x, 0.0f);
	}
	// Needs copies on left side
	else if (rootTransform.LeftBound() > camera->LeftBound()) {
		rootTransform.position -= glm::vec2(rootTransform.scale.x, 0.0f);
	}
}


void LoopingBackground::Render(Renderer* rend) {
	for (int i = 0; i < maxCopies; i++) {
		rend->DrawQuad(texture, rootTransform.position + glm::vec2(i * rootTransform.scale.x, 0.0f), rootTransform.scale);
	}
}