#include "PhysicsController.h"

PhysicsController::PhysicsController(Transform* transform, CollisionGrid* collGrid) 
	: transform(transform), velocity(0.0f), acceleration(0.0f), grid(collGrid){

}

void PhysicsController::Translate(glm::vec2 translation) {
	if (translation.x == 0 && translation.y == 0) return;
	transform->Translate(translation);
	
	for (int i = 0; i < hitboxes.size(); i++) {
		grid->UpdateGridPosition(hitboxes[i]);
	}
}

void PhysicsController::Update(float dt) {
	Translate(dt * velocity);
	velocity += dt * acceleration;
}