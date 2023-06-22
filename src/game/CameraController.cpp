#include "CameraController.h"

CameraController::CameraController(Camera* camera)
	: camera(camera), followTarget(nullptr), followBounds(), paralaxTargets(){}

void CameraController::SetFollowTarget(HitBox* target, float leftBound, float rightBound, float upperBound, float lowerBound) {
	followTarget = target;
	followBounds = HitBox(leftBound, rightBound, upperBound, lowerBound);
	followBounds.parentTransform = &(camera->transform);
}
void CameraController::Update(float dt) {
	// The movement of the camera for this frame
	glm::vec2 camMovement(0.0f);

	// Target is outside of bounds -- move camera
	//if (!followTarget->CheckCollision(followBounds)) {
		//float xDiff = followTarget->GetGlobalPosition().x - followBounds.GetGlobalPosition().x;
		//float yDiff = followTarget->GetGlobalPosition().y - followBounds.GetGlobalPosition().y;
		
		if (followTarget->RightBound() > followBounds.RightBound()) {
			camMovement.x = followTarget->RightBound() - followBounds.RightBound();
		}
		else if (followTarget->LeftBound() < followBounds.LeftBound()) {
			camMovement.x = followTarget->LeftBound() - followBounds.LeftBound();
		}

		if (followTarget->BottomBound() > followBounds.BottomBound()) {
			camMovement.y = followTarget->BottomBound() - followBounds.BottomBound();
		}
		else if (followTarget->TopBound() < followBounds.TopBound()) {
			camMovement.y = followTarget->TopBound() - followBounds.TopBound();
		}

		// Adjusts x based on right or left bounds 
		//if (xDiff > 0) camMovement.x = followTarget->LeftBound() - followBounds.RightBound();
		//else camMovement.x = followTarget->RightBound() - followBounds.LeftBound();
		//// Adjusts y based on top or bottom bounds
		//if (yDiff > 0) camMovement.y = followTarget->TopBound() - followBounds.BottomBound();
		//else camMovement.y = followTarget->BottomBound() - followBounds.TopBound();

	//}
	camera->transform.Translate(camMovement);
	// TODO: do parallax
	// Apply (translate) camMovement * paralaxAmount to each transform
}