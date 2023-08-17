#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "graphics/Transform.h"
#include "physics/CollisionGrid.h"


class PhysicsController
{
public:
	PhysicsController() : transform(nullptr), velocity(0.0f), acceleration(0.0f), grid(nullptr) {}
	PhysicsController(Transform* transform, CollisionGrid* collGrid);
	
	void Update(float dt);

	void Translate(glm::vec2 translation);
	void Translate(float xTrans, float yTrans) { Translate(glm::vec2(xTrans, yTrans)); }
	void SetTransform(Transform* t) { transform = t; }

	// For convenience and readability
	float XSpeed() { return std::abs(velocity.x); }
	float YSpeed() { return std::abs(velocity.y); }
	float XVelDirection() { if (velocity.x == 0) return 0; return velocity.x > 0 ? 1.0f : -1.0f; }
	float YVelDirection() { if (velocity.y == 0) return 0; return velocity.y > 0 ? 1.0f : -1.0f; }

	glm::vec2 velocity;
	glm::vec2 acceleration;
	// Multiplies movement by this
	glm::vec2 multiplier = glm::vec2(1.0f,1.0f);
	// Dynamic hitboxes to be updated in grid
	std::vector<int> hitboxes;
	CollisionGrid* grid;
private:
	Transform* transform;

};

