#pragma once
#include "scene/Entity.h"
#include "physics/HitBox.h"
#include "physics/CollisionGrid.h"

#define MAX_SPITBALLS 5

struct SpitBall {
	bool active;
	Transform transform;
	HitBox* hitBox;
};

class Bully : public Entity
{
public:
	// Shouldn't be explicitly called
	Bully() {}
	Bully(float xPos, float yPos, CollisionGrid* grid, const std::string name = "bully", float xScale = 1.0f, float yScale = 1.0f, float rotation = 0.0);

	
	void Render(Renderer* renderer) override;
	void Update(float dt) override;

	float fireRate = 2.0f;
	float ballSpeed = 2.0f;

private:

	void FireSpitBall();

	Texture* texture = nullptr;
	SubTexture subTexture;

	// Only 5 spitballs can exist on screen at a time
	SpitBall spitBalls[MAX_SPITBALLS];

	// Keeps track of the index of spitBalls that should be used to instantiate the next spitball (loops around after 5) 
	int nextSpitball = 0;
	// Signifies when the next spitball should be shot
	float spitBallTimer = 0.0f;

};

