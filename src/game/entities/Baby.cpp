#include "Baby.h"

// Ground constants
#define MAX_GROUND_VELOCITY 4.0f
#define GROUND_ACCELERATION 3.0f
#define GROUND_FRICTION 1.0f
// Jump/air constants
#define MIN_JUMP_VEL 1.0f
#define JUMP_VEL 3.0f
#define JUMP_CHARGE_RATE 15.0f
#define JUMP_X_VEL 0.5f
#define FALLSPEED 6.0f
#define FASTFALLSPEED 13.0f
// Rail constants
#define GRIND_BONUS_SPEED 0.5f
#define GRIND_MAX_SPEED 5.5f

// Balance meter
#define METER_HEIGHT 0.12f
#define BALANCE_TILT_FACTOR 0.8f
#define BALANCE_TILT_SPEED 4.0f
#define BALANCE_TILT_SENSITIVITY 0.9f
#define BALANCE_NOISE 3.0f

#define ANIM_SPEED 7.0f

Animation Baby::idle;
Animation Baby::ride;
Animation Baby::crouch;
Animation Baby::grind;
Animation Baby::jumpAscend;
Animation Baby::jumpDescend;



void OnBodyCollision(const HitBox& thisHitBox, const HitBox& otherHitBox) {

}

void OnBoardCollision(const HitBox& thisHitBox, const HitBox& otherHitBox) {
	if (otherHitBox.tag == HitBoxType::GrindRail) {
		((Baby*)thisHitBox.parentEntity)->touchingRail = true;
		((Baby*)thisHitBox.parentEntity)->railY = otherHitBox.BottomBound();
	}
}

Baby::Baby(float xPos, float yPos, float xScale, float yScale, float rotation, CollisionGrid* grid, const std::string name)
	: Entity(xPos, yPos, xScale, yScale, rotation, name), physicsController(&transform, grid), state(BabyState::Ground),
	direction(1.0f), nextJumpVel(MIN_JUMP_VEL){

	texture = ResourceManager::GetTexture("baby");
	meterSubTex = SubTexture(texture, 1 * 128, 4 * 128 + 40, 128, 64);
	indicatorSubTex = SubTexture(texture, 1 * 128, 4 * 128 + 40+64, 128, 64);
	InitializeAnimations();
	// Initialize hitboxes
	bodyHitBox = grid->Register(HitBox(0.0f, 0.0f, 0.45f, 0.8f, this, OnBodyCollision));
	boardHitBox = grid->Register(HitBox(0.0f, 0.4f, 0.6f, 0.2f, this, OnBoardCollision));
	physicsController.hitboxes.push_back(bodyHitBox->id);
	physicsController.hitboxes.push_back(boardHitBox->id);
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

	ride.SetPlaySpeed(ANIM_SPEED + 6.0f);
	ride.AddFrame(Frame(SubTexture(texture, 0 * 128, 128 + 10, 128, 128)));
	//ride.AddFrame(Frame(SubTexture(texture, 1 * 128, 128 + 10, 128, 128)));
	ride.AddFrame(Frame(SubTexture(texture, 2 * 128, 128 + 10, 128, 128)));
	//ride.AddFrame(Frame(SubTexture(texture, 3 * 128, 128 + 10, 128, 128)));
	ride.AddFrame(Frame(SubTexture(texture, 4 * 128, 128 + 10, 128, 128)));
	ride.AddFrame(Frame(SubTexture(texture, 5 * 128, 128 + 10, 128, 128)));
	ride.AddFrame(Frame(SubTexture(texture, 6 * 128, 128 + 10, 128, 128)));
	ride.AddFrame(Frame(SubTexture(texture, 7 * 128, 128 + 10, 128, 128)));
	ride.AddFrame(Frame(SubTexture(texture, 0 * 128, 2 * 128 + 20, 128, 128)));
	ride.AddFrame(Frame(SubTexture(texture, 1 * 128, 2 * 128 + 20, 128, 128)));
	ride.AddFrame(Frame(SubTexture(texture, 2 * 128, 2 * 128 + 20, 128, 128)));
	ride.AddFrame(Frame(SubTexture(texture, 3 * 128, 2 * 128 + 20, 128, 128)));
	ride.AddFrame(Frame(SubTexture(texture, 4 * 128, 2 * 128 + 20, 128, 128)));
	animator.AddAnimation("ride", &ride);

	crouch.SetPlaySpeed(ANIM_SPEED);
	crouch.AddFrame(Frame(SubTexture(texture, 6 * 128, 0, 128, 128)));
	animator.AddAnimation("crouch", &crouch);

	grind.SetPlaySpeed(ANIM_SPEED - 2.0f);
	grind.AddFrame(Frame(SubTexture(texture, 5 * 128, 2 * 128 + 20, 128, 128)));
	grind.AddFrame(Frame(SubTexture(texture, 6 * 128, 2 * 128 + 20, 128, 128)));
	grind.AddFrame(Frame(SubTexture(texture, 7 * 128, 2 * 128 + 20, 128, 128)));
	grind.AddFrame(Frame(SubTexture(texture, 6 * 128, 2 * 128 + 20, 128, 128)));
	animator.AddAnimation("grind", &grind);

	jumpAscend.SetPlaySpeed(ANIM_SPEED + 8.0f);
	jumpAscend.AddFrame(Frame(SubTexture(texture, 0 * 128, 3 * 128 + 30, 128, 128)));
	//jump.AddFrame(Frame(SubTexture(texture, 1 * 128, 3 * 128 + 30, 128, 128)));
	jumpAscend.AddFrame(Frame(SubTexture(texture, 2 * 128, 3 * 128 + 30, 128, 128)));
	jumpAscend.AddFrame(Frame(SubTexture(texture, 2 * 128, 3 * 128 + 30, 128, 128)));
	jumpAscend.AddFrame(Frame(SubTexture(texture, 3 * 128, 3 * 128 + 30, 128, 128)));
	jumpAscend.AddFrame(Frame(SubTexture(texture, 4 * 128, 3 * 128 + 30, 128, 128)));
	jumpAscend.AddFrame(Frame(SubTexture(texture, 5 * 128, 3 * 128 + 30, 128, 128)));
	jumpAscend.AddFrame(Frame(SubTexture(texture, 6 * 128, 3 * 128 + 30, 128, 128)));
	jumpAscend.AddFrame(Frame(SubTexture(texture, 7 * 128, 3 * 128 + 30, 128, 128)));
	animator.AddAnimation("jumpAscend", &jumpAscend);

	jumpDescend.SetPlaySpeed(ANIM_SPEED);
	jumpDescend.AddFrame(Frame(SubTexture(texture, 0 * 128, 4 * 128 + 40, 128, 128)));
	animator.AddAnimation("jumpDescend", &jumpDescend);
}


void Baby::Render(Renderer* renderer) {
	if (generateSparks) {
		float intensity = physicsController.XSpeed() / GRIND_MAX_SPEED;
		glm::vec2 pos = glm::vec2(-(transform.scale.x / 4.0f), transform.scale.y / 2.0f - boardHitBox->localTransform.scale.y / 2.0f);
		sparks.GenerateSparks(transform.position + pos, direction, renderer, intensity);
	}
	renderer->DrawQuad(texture, subTexture, transform.position, transform.scale);
	if(balancing) RenderBalanceMeter(renderer);
	//bodyHitBox->Render(renderer);
	//boardHitBox->Render(renderer);
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
	case BabyState::Grind:
		GrindUpdate(dt);
		break;
	case BabyState::FallOffRail:
		FallOfRailUpdate(dt);
		break;
	}


	physicsController.Update(dt);
	animator.Update(dt);
	subTexture = animator.GetCurrentFrame().subTexture;
	// Assumes collision checks are done before update
	touchingRail = false;
}


void Baby::GroundedUpdate(float dt) {
	// Gets directional input
	float inputDir = InputDirection();
	if (inputDir && !InputCrouch()) {
		// Direction player is facing
		direction = inputDir;
		// Directio player is actually moving
		float velSign = physicsController.velocity.x >= 0 ? 1.0f : -1.0f;
		bool atMaxVel = std::abs(physicsController.velocity.x) >= MAX_GROUND_VELOCITY;
		animator.PlayOnce("ride", true, true);
		transform.SetScaleX(direction);
		// Hit max velocity -> set velocity to max
		if (atMaxVel) {
			physicsController.velocity.x = velSign * MAX_GROUND_VELOCITY;
			physicsController.acceleration.x = 0;
		}
		// Hasn't hit max vel -> apply acceleration
		if(!atMaxVel || direction !=velSign){
			physicsController.acceleration.x = direction * GROUND_ACCELERATION;
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

	// Crouch
	if (InputCrouch()) {
		animator.PlayOnce("crouch", true, true);
		// Adjust hitbox to crouch
		bodyHitBox->localTransform.SetScaleY(0.7f);
		bodyHitBox->localTransform.SetPositionY(0.05f);
		// Charge jump
		nextJumpVel = std::min(nextJumpVel + JUMP_CHARGE_RATE*dt, JUMP_VEL);
	}
	// Jump
	else if (InputJump()) {
		// Restore hitbox
		bodyHitBox->localTransform.SetScaleY(0.8f);
		bodyHitBox->localTransform.SetPositionY(0.0f);
		// Add velocity and restore velocity variable to its minimum
		physicsController.velocity.y = -nextJumpVel;
		nextJumpVel = MIN_JUMP_VEL;
		// Set custom x vel and stop any deceleration
		physicsController.acceleration.x = 0;
		physicsController.velocity.x += JUMP_X_VEL * direction;
		state = BabyState::Air;
	}
}

void Baby::AirUpdate(float dt) {
	// Grind
	if (touchingRail && InputGrind()) {
		// Find new velocity with correct sign
		float xVel = std::min(std::abs(physicsController.velocity.x) + GRIND_BONUS_SPEED, GRIND_MAX_SPEED);
		float sign = physicsController.XVelDirection();
		// Set velocity, and zero y vel and all acceleration
		physicsController.velocity.x = xVel * sign;
		physicsController.velocity.y = 0.0f;
		physicsController.acceleration = glm::vec2(0.0f);
		// Place baby on rail
		transform.SetPositionY(railY-transform.GetScale().y/2.0f);
		
		state = BabyState::Grind;
		animator.PlayOnce("grind", true, true);
	}
	// In the air
	else if (transform.GetPosition().y < 0) {
		// Changes to fastfall speed at peak of jump
		if (physicsController.velocity.y < 0) {
			physicsController.acceleration.y = FALLSPEED;
			animator.PlayOnce("jumpAscend", false, true);
		}
		else {
			physicsController.acceleration.y = FASTFALLSPEED;
			animator.PlayOnce("jumpDescend", false, true);
		}
	}
	// On the "ground"
	else {
		transform.SetPositionY(0);
		physicsController.velocity.y = 0;
		physicsController.acceleration.y = 0;
		state = BabyState::Ground;
		balance = 0.0f;
	}
}

void Baby::GrindUpdate(float dt) {
	// Create epic sparks 
	generateSparks = true;
	balancing = true;
	UpdateBalanceMeter(dt);
	// End of rail
	if (!touchingRail) {
		state = BabyState::Air;
		generateSparks = false;
		balancing = false;
	}
	// Lost balance
	else if (std::abs(balance) >= 1.0f) {
		state = BabyState::FallOffRail;
		generateSparks = false;
		balancing = false;
	}
	// Crouch
	else if (InputCrouch()) {
		animator.PlayOnce("crouch", true, true);
		bodyHitBox->localTransform.SetScaleY(0.7f);
		bodyHitBox->localTransform.SetPositionY(0.05f);
		// Charge jump
		nextJumpVel = std::min(nextJumpVel + JUMP_CHARGE_RATE * dt, JUMP_VEL);
	}
	// Jump
	else if (InputJump()) { // <-- think about if grind should still have charge/hold jumps
		bodyHitBox->localTransform.SetScaleY(0.8f);
		bodyHitBox->localTransform.SetPositionY(0.0f);
		physicsController.velocity.y = -nextJumpVel;
		nextJumpVel = MIN_JUMP_VEL;
		physicsController.acceleration.x = 0;
		physicsController.velocity.x += JUMP_X_VEL * physicsController.XVelDirection();
		state = BabyState::Air;
		generateSparks = false;
		balancing = false;
	}

}

void Baby::FallOfRailUpdate(float dt) {
	// In air
	if (transform.GetPosition().y < 0) {
		physicsController.acceleration.y = FASTFALLSPEED;
	}
	// On the "ground"
	else {
		transform.SetPositionY(0);
		physicsController.velocity.y = 0;
		physicsController.velocity.x = 0;
		physicsController.acceleration.y = 0;
	}
	// JUST FOR DEBUGGING
	if (InputManager::GetGamepadButton(GLFW_GAMEPAD_BUTTON_START)) {
		state = BabyState::Ground;
		balance = 0.0f;
	}
}

void Baby::UpdateBalanceMeter(float dt) {
	// Follow tilt of board
	balance += BALANCE_TILT_SPEED * BALANCE_TILT_FACTOR * balance * dt;
	// User input
	balance += InputDirectionRaw() * dt * BALANCE_TILT_SPEED * BALANCE_TILT_SENSITIVITY;
	// Randomness
	balance += (((float)2.0f * std::rand() / (float)RAND_MAX) - 1.0f) * BALANCE_NOISE * dt * BALANCE_TILT_SPEED;
	balance = std::clamp(balance, -1.0f, 1.0f);
	indicatorPos.x = balance/2.5f;
	indicatorPos.y = std::pow(balance/ 2.5f, 2);
}

void Baby::RenderBalanceMeter(Renderer* renderer) {
	renderer->DrawQuad(texture, meterSubTex, glm::vec2(transform.position.x, transform.TopBoundary() - METER_HEIGHT), glm::vec2(1.0f, 0.5f));
	renderer->DrawQuad(texture, indicatorSubTex,
		glm::vec2(transform.position.x, transform.TopBoundary() - METER_HEIGHT) + indicatorPos, glm::vec2(1.0f, 0.5f));
}

float Baby::InputDirection() {
	// If there is keyboard input
	if (InputManager::GetKey(GLFW_KEY_D)) return 1.0f;
	if (InputManager::GetKey(GLFW_KEY_A)) return -1.0f;
	// If there is gamepad input 
	float axisInput = InputManager::GetGamepadAxisDigital(GLFW_GAMEPAD_AXIS_LEFT_X);
	if (axisInput) return axisInput;
	// No directional input
	return 0.0f;
}

float Baby::InputDirectionRaw() {
	// If there is gamepad input 
	float axisInput = InputManager::GetGamepadAxisRaw(GLFW_GAMEPAD_AXIS_LEFT_X);
	if (axisInput) return axisInput;
	// If there is keyboard input
	if (InputManager::GetKey(GLFW_KEY_D)) return 1.0f;
	if (InputManager::GetKey(GLFW_KEY_A)) return -1.0f;

	return 0.0f;
}

bool Baby::InputCrouch() {
	return InputManager::GetKey(GLFW_KEY_SPACE) || InputManager::GetGamepadButton(GLFW_GAMEPAD_BUTTON_A);
}

bool Baby::InputJump() {
	return InputManager::GetKeyUp(GLFW_KEY_SPACE) || InputManager::GetGamepadButtonUp(GLFW_GAMEPAD_BUTTON_A);
}

bool Baby::InputGrind() {
	return InputManager::GetKeyDown(GLFW_KEY_LEFT_SHIFT) || InputManager::GetGamepadButtonDown(GLFW_GAMEPAD_BUTTON_Y);
}

/*
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
}*/
