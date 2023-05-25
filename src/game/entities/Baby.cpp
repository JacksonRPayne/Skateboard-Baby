#include "Baby.h"

// Ground constants
#define MAX_GROUND_VELOCITY 4.0f
#define GROUND_ACCELERATION 3.0f
#define GROUND_FRICTION 1.0f
// Jump/air constants
#define JUMP_VEL 3.0f
#define JUMP_X_VEL 0.5f
#define FALLSPEED 6.0f
#define FASTFALLSPEED 13.0f

#define ANIM_SPEED 7.0f

Animation Baby::idle;
Animation Baby::ride;


void OnBodyCollision(const HitBox& thisHitBox, const HitBox& otherHitBox) {

}

void OnBoardCollision(const HitBox& thisHitBox, const HitBox& otherHitBox) {

}

Baby::Baby(float xPos, float yPos, float xScale, float yScale, float rotation, const std::string name)
	: Entity(xPos, yPos, xScale, yScale, rotation, name), physicsController(&transform), state(BabyState::Ground),
	direction(1.0f), bodyHitBox(0.0f, 0.0f, 0.45f, 0.8f, this, OnBodyCollision), boardHitBox(0.0f, 0.4f, 0.6f, 0.2f, this, OnBoardCollision) {
	texture = ResourceManager::GetTexture("baby");
	InitializeAnimations();
}

void Baby::InitializeAnimations() {
	idle.SetPlaySpeed(ANIM_SPEED);
	idle.AddFrame(Frame(SubTexture(texture, 0 * 128, 0, 128, 128)));
	idle.AddFrame(Frame(SubTexture(texture, 1 * 128, 0, 128, 128)));
	idle.AddFrame(Frame(SubTexture(texture, 2 * 128, 0, 128, 128)));
	idle.AddFrame(Frame(SubTexture(texture, 3 * 128, 0, 128, 128)));
	idle.AddFrame(Frame(SubTexture(texture, 4 * 128, 0, 128, 128)));
	idle.AddFrame(Frame(SubTexture(texture, 5 * 128, 0, 128, 128)));
	animator.AddAnimation("idle", &idle);

	ride.SetPlaySpeed(ANIM_SPEED+3.0f);
	ride.AddFrame(Frame(SubTexture(texture, 0 * 128, 128 + 10, 128, 128)));
	//ride.AddFrame(Frame(SubTexture(texture, 1 * 128, 128 + 10, 128, 128)));
	ride.AddFrame(Frame(SubTexture(texture, 2 * 128, 128 + 10, 128, 128)));
	//ride.AddFrame(Frame(SubTexture(texture, 3 * 128, 128 + 10, 128, 128)));
	ride.AddFrame(Frame(SubTexture(texture, 4 * 128, 128 + 10, 128, 128)));
	ride.AddFrame(Frame(SubTexture(texture, 5 * 128, 128 + 10, 128, 128)));
	ride.AddFrame(Frame(SubTexture(texture, 6 * 128, 128 + 10, 128, 128)));
	ride.AddFrame(Frame(SubTexture(texture, 7 * 128, 128 + 10, 128, 128)));

	animator.AddAnimation("ride", &ride);
}


void Baby::Render(Renderer* renderer) {
	renderer->DrawQuad(texture, subTexture, transform.GetModelMatrix());
	//bodyHitBox.Render(renderer);
	//boardHitBox.Render(renderer);
}

void Baby::Update(float dt) {

	switch (state)
	{
	case BabyState::Ground:
		GroundedUpdate(dt);
		break;
	case BabyState::Air:
		AirUpdate(dt);
		break;
	}


	physicsController.Update(dt);
	animator.Update(dt);
	subTexture = animator.GetCurrentFrame().subTexture;
}


void Baby::GroundedUpdate(float dt) {

	if (InputManager::GetKey(GLFW_KEY_D)) {
		direction = 1.0f;
		animator.PlayOnce("ride", true, true);
		transform.SetScaleX(direction);
		// Hit max velocity -> set velocity to max
		if (physicsController.velocity.x >= MAX_GROUND_VELOCITY) {
			physicsController.velocity.x = MAX_GROUND_VELOCITY;
			physicsController.acceleration.x = 0;
		}
		// Hasn't hit max vel -> apply acceleration
		else {
			physicsController.acceleration.x = GROUND_ACCELERATION;
		}
	}
	else if (InputManager::GetKey(GLFW_KEY_A)) {
		direction = -1.0f;
		animator.PlayOnce("ride", true, true);
		transform.SetScaleX(direction);
		if (physicsController.velocity.x <= -MAX_GROUND_VELOCITY) {
			physicsController.velocity.x = -MAX_GROUND_VELOCITY;
			physicsController.acceleration.x = 0;
		}
		else {
			physicsController.acceleration.x = -GROUND_ACCELERATION;
		}
	}
	// No key pressed but moving -> apply friction
	else if (std::abs(physicsController.velocity.x > 0)) { 
		animator.PlayOnce("idle", true, true);
		physicsController.acceleration.x = -GROUND_FRICTION;
	}
	else if (std::abs(physicsController.velocity.x < 0)) {
		animator.PlayOnce("idle", true, true);
		physicsController.acceleration.x = GROUND_FRICTION;
	}
	// Completely still
	else {
		animator.PlayOnce("idle", true, true);
		physicsController.acceleration.x = 0;
	}

	if (InputManager::GetKeyUp(GLFW_KEY_SPACE)) {
		physicsController.velocity.y = -JUMP_VEL;
		physicsController.acceleration.x = 0;
		physicsController.velocity.x += JUMP_X_VEL * direction;
		state = BabyState::Air;
	}
}

void Baby::AirUpdate(float dt) {
	// In the air
	if (transform.GetPosition().y < 0) {
		// Changes to fastfall speed at peak of jump
		if(physicsController.velocity.y < 0) physicsController.acceleration.y = FALLSPEED;
		else physicsController.acceleration.y = FASTFALLSPEED;
	}
	// On the "ground"
	else {
		transform.SetPositionY(0);
		physicsController.velocity.y = 0;
		physicsController.acceleration.y = 0;
		state = BabyState::Ground;
	}
}


Baby::Baby(Baby&& other) noexcept {
	this->transform = other.transform;
	this->name = "Baby";
	this->texture = other.texture;
	this->physicsController = PhysicsController(&(this->transform));
	this->state = other.state;
	this->direction = other.direction;
	this->bodyHitBox = other.bodyHitBox;
	this->bodyHitBox.parentEntity = this;
	this->boardHitBox = other.boardHitBox;
	this->boardHitBox.parentEntity = this;
	// This may break with multiple baby instances
	this->animator = std::move(other.animator);
}

Baby& Baby::operator=(Baby&& other) noexcept {
	if (this != &other) {
		this->transform = other.transform;
		this->name = "Baby";
		this->texture = other.texture;
		this->physicsController = PhysicsController(&(this->transform));
		this->state = other.state;
		this->direction = other.direction;
		this->bodyHitBox = other.bodyHitBox;
		this->bodyHitBox.parentEntity = this;
		this->boardHitBox = other.boardHitBox;
		this->boardHitBox.parentEntity = this;
		this->animator = std::move(other.animator);
	}

	return *this;
}
