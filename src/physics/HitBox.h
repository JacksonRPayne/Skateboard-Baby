#pragma once
#include <string>
#include "scene/Entity.h"
#include "graphics/Transform.h"

// Groups hitboxes into types
enum class HitBoxType {
	None = 0,
	Ground = 1,
	Player = 2
};

struct HitBox
{
	// --Basic Functions--
	HitBox();
	HitBox(float xPos, float yPos, float xScale, float yScale, Entity* parent,
		void(*callback)(const HitBox& thisHitBox, const HitBox& otherHitBox),  HitBoxType tag=HitBoxType::None, bool active=true);

	// Checks collision with another hitbox and calls parent entity callback
	// TODO: idea: give ability to pass custom collision detection function as fptr. Would be good for things like ramps
	bool CheckCollision(const HitBox& other);
	// Checks if hitbox contains point
	bool Contains(glm::vec2 point);
	// Renders outline of hitbox (DEBUGGING ONLY, performs TERRIBLY so NEVER call outside debuggin purposes)
	void Render(Renderer* renderer);

	// Gets the position in relation to the world
	glm::vec2 GetGlobalPosition() const;
	
	// --Variables--
	bool active;
	// Stores the position in relation to the parent entity
	Transform localTransform;
	Entity* parentEntity;
	HitBoxType tag;
	// Calls function on a collision
	void(*collisionCallback)(const HitBox& thisHitBox, const HitBox& otherHitBox);
	// For linkedlist within each collision grid cell
	HitBox* next;

	// --Specified Getters--
	// Corners
	glm::vec2 TopLeft() { return GetGlobalPosition() + glm::vec2(-localTransform.GetScale().x / 2.0f, -localTransform.GetScale().y / 2.0f); }
	glm::vec2 TopRight() { return GetGlobalPosition() + glm::vec2(localTransform.GetScale().x / 2.0f, -localTransform.GetScale().y / 2.0f); }
	glm::vec2 BottomLeft() { return GetGlobalPosition() + glm::vec2(-localTransform.GetScale().x / 2.0f, localTransform.GetScale().y / 2.0f); }
	glm::vec2 BottomRight() { return GetGlobalPosition() + glm::vec2(localTransform.GetScale().x / 2.0f, localTransform.GetScale().y / 2.0f); }
	// Edges
	float TopBound() { return GetGlobalPosition().y - (localTransform.GetScale().y / 2.0f); }
	float BottomBound(){ return GetGlobalPosition().y + (localTransform.GetScale().y / 2.0f); }
	float LeftBound(){ return GetGlobalPosition().x - (localTransform.GetScale().x / 2.0f); }
	float RightBound(){ return GetGlobalPosition().x + (localTransform.GetScale().x / 2.0f); }

};
