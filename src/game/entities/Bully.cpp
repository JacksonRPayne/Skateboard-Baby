#include "Bully.h"

Bully::Bully(float xPos, float yPos, CollisionGrid* grid, const std::string name , float xScale, float yScale , float rotation) :
	Entity(xPos, yPos, xScale, yScale, rotation, name), spitBalls()
{
	texture = ResourceManager::GetTexture("bully");
	subTexture = SubTexture(texture, 0, 0, 128, 128);

	for (int i = 0; i < MAX_SPITBALLS; i++) {
		spitBalls[i].active = false;
		// Defines each transform starting at the bullys position
		spitBalls[i].transform = Transform(transform.position.x, transform.position.y, 0.5f, 0.5f, 0.0f);
		spitBalls[i].hitBox = grid->Register(HitBox(0.0f, 0.0f, 0.15f, 0.15f, nullptr, nullptr, nullptr, HitBoxType::SpitBall));
		// Links each hitbox to each transform
		spitBalls[i].hitBox->parentTransform = &spitBalls[i].transform;
	}
}

void Bully::Render(Renderer* renderer) {
	// Render spitballs
	SubTexture spitball(texture, 128, 0, 64, 64);
	for (int i = 0; i < MAX_SPITBALLS; i++) {
		// Don't render the spitball if it hasn't been spat yet
		if (spitBalls[i].active)
			renderer->DrawQuad(texture, spitball, spitBalls[i].transform.GetModelMatrix());
	}

	// Render bully
	renderer->DrawQuad(texture, subTexture, transform.position, transform.scale);
}


void Bully::Update(float dt) {

	spitBallTimer -= dt;

	if (spitBallTimer <= 0.0f) {
		FireSpitBall();
	}

	for (int i = 0; i < MAX_SPITBALLS; i++) {
		if (spitBalls[i].active) {
			spitBalls[i].transform.position.x -= ballSpeed * dt;
			spitBalls[i].transform.rotation += (-ballSpeed * 100.0f*dt);
		}
	}
}


void Bully::FireSpitBall() {
	spitBalls[nextSpitball].active = true;
	spitBalls[nextSpitball].transform.position = transform.position;

	spitBallTimer = fireRate;
	nextSpitball = (nextSpitball + 1) % MAX_SPITBALLS;
}