#pragma once
#include "scene/Entity.h"
#include "system/ResourceManager.h"
#include "physics/PhysicsController.h"
#include "physics/HitBox.h"
#include "system/InputManager.h"
#include "animation/Animator.h"


enum class BabyState {
	Ground = 0,
	Air = 1,
	Grind = 2
};

class Baby : public Entity
{
public:
	// It can be assumed that this will never be *explicitly* called
	Baby() : texture(nullptr), direction(0.0f), state(BabyState::Ground){}
	Baby(float xPos, float yPos, float xScale, float yScale, float rotation, const std::string name = "Baby");
	Baby(Baby&& other)noexcept;
	Baby& operator=(Baby&& other) noexcept;
	
	void Render(Renderer* renderer) override;
	void Update(float dt) override;

	// Components
	HitBox bodyHitBox;
	HitBox boardHitBox;
	PhysicsController physicsController;
	

	// State variables
	BabyState state;
	float direction;
	Animator animator;
	bool touchingRail = false;
	float railY = -1.0f;
	float nextJumpVel;

private:
	// Update for different states
	void GroundedUpdate(float dt);
	void AirUpdate(float dt);
	void GrindUpdate(float dt);
	// Animations
	void InitializeAnimations();
	static Animation idle;
	static Animation ride;
	static Animation crouch;

	Texture* texture;
	SubTexture subTexture;
};

