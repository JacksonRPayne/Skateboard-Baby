#include "CameraController.h"

CameraController::CameraController(Camera* camera)
	: camera(camera), followTarget(nullptr), followBounds(), paralaxTargets(){}

void CameraController::SetFollowTarget(HitBox* target, float leftBound, float rightBound, float upperBound, float lowerBound) {
	followTarget = target;
	followBounds = HitBox(leftBound, rightBound, upperBound, lowerBound);
	followBounds.parentTransform = &(camera->transform);
}
void CameraController::Update(float dt) {
	// Target is outside of bounds -- move camera
	if (!followTarget->CheckCollision(followBounds)) {
		float xDiff = followTarget->GetGlobalPosition().x - followBounds.GetGlobalPosition().x;
		float yDiff = followTarget->GetGlobalPosition().y - followBounds.GetGlobalPosition().y;

		if (std::abs(xDiff) > std::abs(yDiff)) {
			camera->transform.Translate(xDiff, 0.0f);
		}
		else {
			camera->transform.Translate(0.0f, yDiff);
		}
	}
	// TODO: do parallax
}