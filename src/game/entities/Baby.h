#pragma once
#include "scene/Entity.h"
#include "system/ResourceManager.h"
#include "physics/PhysicsController.h"
#include "physics/HitBox.h"
#include "system/InputManager.h"
#include "animation/Animator.h"
#include "game/Sparks.h"

enum class BabyState {
	Ground = 0,
	Air = 1,
	Grind = 2,
	FallOffRail = 3
};

class Baby : public Entity
{
public:
	// It can be assumed that this will never be *explicitly* called
	Baby() : texture(nullptr), direction(0.0f), state(BabyState::Ground), nextJumpVel(0.0f){}
	Baby(float xPos, float yPos, float xScale, float yScale, float rotation, const std::string name = "Baby");
	// Baby is too fat to copy or move, use placement new
	Baby(Baby& other) = delete;
	Baby& operator=(Baby& other) = delete;
	Baby(Baby&& other) = delete;
	Baby& operator=(Baby&& other) = delete;
	
	void Render(Renderer* renderer) override;
	void Update(float dt) override;

	// ----Components----
	HitBox bodyHitBox;
	HitBox boardHitBox;
	PhysicsController physicsController;
	
	// ----State variables----
	BabyState state;
	// For facing direction not moving direction
	float direction;
	Animator animator;
	// Rail stuff
	bool touchingRail = false;
	float railY = -1.0f;
	// For jump charging
	float nextJumpVel;

private:
	// Update for different states
	void GroundedUpdate(float dt);
	void AirUpdate(float dt);
	void GrindUpdate(float dt);
	void UpdateBalanceMeter(float dt);
	void FallOfRailUpdate(float dt);

	// Meter stuff
	void RenderBalanceMeter(Renderer* renderer);
	SubTexture meterSubTex; // For the meter itself
	SubTexture indicatorSubTex; // The little baby face
	bool balancing = false;
	glm::vec2 indicatorPos = glm::vec2(0.0f);
	float balance = 0.0f; // -1/1 == fall

	// Abstracted input for support of multiple controllers
	bool InputCrouch();
	bool InputJump();
	bool InputGrind();
	// Returns direction (-1, 1)
	float InputDirection();
	// Returns analog direction if available (vals in btwn -1, 1)
	float InputDirectionRaw();

	// Animations
	void InitializeAnimations();
	static Animation idle;
	static Animation ride;
	static Animation crouch;
	static Animation grind;
	static Animation jumpAscend;
	static Animation jumpDescend;
	// Sparks :D
	Sparks sparks;
	bool generateSparks = false;

	Texture* texture;
	SubTexture subTexture;
};

