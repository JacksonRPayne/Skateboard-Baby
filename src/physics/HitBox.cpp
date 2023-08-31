#include "Hitbox.h"

HitBox::HitBox()
: localTransform(), parentEntity(nullptr), parentTransform(nullptr), 
	collisionCallback(nullptr), collisionExitCallback(nullptr), tag(HitBoxType::None), active(true){}

HitBox::HitBox(float xPos, float yPos, float xScale, float yScale, Entity* parent,
	void(*callback)(const HitBox& thisHitBox, const HitBox& otherHitBox), 
	void(*exitCallback)(const HitBox& thisHitBox, const HitBox& otherHitBox), HitBoxType tag)
	: localTransform(xPos, yPos, xScale, yScale, 0.0f), parentEntity(parent), collisionCallback(callback), 
	collisionExitCallback(exitCallback), tag(tag), parentTransform(nullptr) {
}

HitBox::HitBox(float leftBound, float rightBound, float upperBound, float lowerBound, HitBoxType tag)
	: localTransform((rightBound - leftBound) / 2.0f + leftBound, (lowerBound - upperBound) / 2.0f + upperBound, (rightBound - leftBound), (lowerBound - upperBound), 0.0f),
	parentEntity(nullptr), parentTransform(nullptr), collisionCallback(nullptr), collisionExitCallback(nullptr), tag(tag){
}

bool HitBox::CheckCollision(const HitBox& other) {
	// Checks edge cases
	if (this == &other) return false;
	if (!this->active || !other.active) return false;
	
	// Here its assumed that it's only ever a box checking collision, and only what its checking against might be different
	bool collision = other.collisionCheck(*this, other);
	
	if (collision) {
		if (collisionCallback) (*collisionCallback)(*this, other);
	}

	return collision;
}

bool HitBox::Contains(glm::vec2 point) {
	glm::vec2 pos = GetGlobalPosition();
	glm::vec2 size = 0.5f * localTransform.GetScale();

	return (point.x > pos.x - size.x) && (point.x < pos.x + size.x) && (point.y < pos.y + size.y) && (point.y > pos.y - size.y);
}

glm::vec2 HitBox::GetGlobalPosition() const{
	// Option to have or not have parent entity
	if (parentEntity) return parentEntity->transform.GetPosition() + localTransform.GetPosition();
	else if (parentTransform) return parentTransform->position + localTransform.position;
	return localTransform.GetPosition();
}

void HitBox::Render(Renderer* renderer) {
	renderer->DrawLine(TopLeft(), TopRight(), 5.0f, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	renderer->DrawLine(TopRight(), BottomRight(), 5.0f, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	renderer->DrawLine(BottomRight(), BottomLeft(), 5.0f, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	renderer->DrawLine(BottomLeft(), TopLeft(), 5.0f, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
}


bool HitBox::BoxCollisionCheck(const HitBox& thisHitBox, const HitBox& otherHitBox) {

	// Stores positions
	glm::vec2 otherPos = otherHitBox.GetGlobalPosition();
	glm::vec2 pos = thisHitBox.GetGlobalPosition();
	// Stores half size for easier border calculation
	glm::vec2 otherSize = 0.5f * otherHitBox.localTransform.GetScale();
	glm::vec2 size = 0.5f * thisHitBox.localTransform.GetScale();

	bool collision = (pos.x + size.x > otherPos.x - otherSize.x) &&
		(pos.x - size.x < otherPos.x + otherSize.x) &&
		(pos.y + size.y > otherPos.y - otherSize.y) &&
		(pos.y - size.y < otherPos.y + otherSize.y);

	return collision;
}

bool HitBox::UpRampCollisionCheck(const HitBox& thisHitBox, const HitBox& otherHitBox) {
	float xDiff = thisHitBox.BottomRight().x - otherHitBox.LeftBound();
	float yDiff = thisHitBox.BottomRight().y - otherHitBox.TopBound();

	// It makes sense... draw diagram
	// Using width here but its assumed that the collision box is a square and the ramp runs its diagonal
	return BoxCollisionCheck(thisHitBox, otherHitBox) && (xDiff + yDiff) > (otherHitBox.localTransform.scale.x);
}

bool HitBox::DownRampCollisionCheck(const HitBox& thisHitBox, const HitBox& otherHitBox) {
	float xDiff = thisHitBox.BottomLeft().x - otherHitBox.LeftBound();
	float yDiff = thisHitBox.BottomLeft().y - otherHitBox.TopBound();

	// This one also hard to explain.... draw a diagram and remember that y increases downwardly
	return BoxCollisionCheck(thisHitBox, otherHitBox) && yDiff > xDiff;
}

glm::vec2 HitBox::ResolveUpRampX(const HitBox& thisHitBox, const HitBox& otherHitBox) {
	// min prevents popping at the top
	float xDiff = std::min(thisHitBox.BottomRight().x - otherHitBox.LeftBound(), otherHitBox.localTransform.scale.x);
	float yDiff = std::min(thisHitBox.BottomRight().y - otherHitBox.TopBound(), otherHitBox.localTransform.scale.y);

	float diff = xDiff + yDiff;

	// I dont remember how I figured this out
	return glm::vec2(otherHitBox.localTransform.scale.x - diff, 0.0f);
}

glm::vec2 HitBox::ResolveUpRampY(const HitBox& thisHitBox, const HitBox& otherHitBox) {
	float xDiff = std::min(thisHitBox.BottomRight().x - otherHitBox.LeftBound(), otherHitBox.localTransform.scale.x);
	float yDiff = std::min(thisHitBox.BottomRight().y - otherHitBox.TopBound(), otherHitBox.localTransform.scale.y);

	float diff = xDiff + yDiff;

	return glm::vec2(0.0f, otherHitBox.localTransform.scale.x - diff);
}

glm::vec2 HitBox::ResolveDownRampX(const HitBox& thisHitBox, const HitBox& otherHitBox){
	// min prevents popping at the top
	float xDiff = std::min(thisHitBox.BottomLeft().x - otherHitBox.LeftBound(), otherHitBox.localTransform.scale.x);
	float yDiff = std::min(thisHitBox.BottomLeft().y - otherHitBox.TopBound(), otherHitBox.localTransform.scale.y);

	float diff = yDiff - xDiff;

	return glm::vec2(diff, 0.0f);
}
glm::vec2 HitBox::ResolveDownRampY(const HitBox& thisHitBox, const HitBox& otherHitBox) {
	// min prevents popping at the top
	float xDiff = std::min(thisHitBox.BottomLeft().x - otherHitBox.LeftBound(), otherHitBox.localTransform.scale.x);
	float yDiff = std::min(thisHitBox.BottomLeft().y - otherHitBox.TopBound(), otherHitBox.localTransform.scale.y);

	float diff = yDiff - xDiff;
	
	return glm::vec2(0.0f, - diff);
}