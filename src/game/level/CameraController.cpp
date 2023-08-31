#include "CameraController.h"

CameraController::CameraController(Camera* camera)
	: camera(camera), followTarget(nullptr), followBounds(), paralaxTargets() {
	minimumPos = camera->transform.position;
}

void CameraController::SetFollowTarget(Transform* target, float leftBound, float rightBound, float upperBound, float lowerBound) {
	followTarget = target;
	followBounds = HitBox(leftBound, rightBound, upperBound, lowerBound);
	followBounds.parentTransform = &(camera->transform);
}

void CameraController::Update(float dt) {
	if (!followTarget) return;
	// The movement of the camera for this frame
	glm::vec2 camMovement(0.0f);

	// Checks if follow target is leaving bounds
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

	// Clamp cam position to minimum bounds
	if ((camera->transform.position + camMovement).x < minimumPos.x) {
		camMovement.x = minimumPos.x - camera->transform.position.x;
	}
	if ((camera->transform.position + camMovement).y > minimumPos.y){
		camMovement.y = minimumPos.y - camera->transform.position.y;
	}

	// Move camera
	camera->transform.Translate(camMovement);
	// Apply parallax
	for (int i = 0; i < paralaxTargets.size(); i++) {
		paralaxTargets[i].transform->Translate(camMovement * paralaxTargets[i].parallaxIntensity);
	}
}