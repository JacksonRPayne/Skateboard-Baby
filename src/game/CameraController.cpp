#include "CameraController.h"

CameraController::CameraController(Camera* camera)
	: camera(camera), followTarget(nullptr), followBounds(), paralaxTargets(), minimumPos(){}

void CameraController::SetFollowTarget(Transform* target, float leftBound, float rightBound, float upperBound, float lowerBound) {
	followTarget = target;
	followBounds = HitBox(leftBound, rightBound, upperBound, lowerBound);
	followBounds.parentTransform = &(camera->transform);
}
void CameraController::Update(float dt) {
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

	if ((camera->transform.position + camMovement).x < minimumPos.x || (camera->transform.position + camMovement).y < minimumPos.y) {
		camMovement = minimumPos - camera->transform.position;
	}
	camera->transform.Translate(camMovement);
	// TODO: do parallax
	// Apply (translate) camMovement * paralaxAmount to each transform
}