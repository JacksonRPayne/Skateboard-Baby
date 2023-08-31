#pragma once
#include <string>
#include "scene/Entity.h"
#include "graphics/Transform.h"

// Groups hitboxes into types
enum class HitBoxType {
	None = 0,
	Ground = 1,
	Player = 2,
	GrindRail = 3,
	UpRamp = 4,
	DownRamp = 5,
	SpitBall = 6
};


struct HitBox
{
	// -- Constructors --
	HitBox();
	HitBox(float xPos, float yPos, float xScale, float yScale, Entity* parent,
		void(*callback)(const HitBox& thisHitBox, const HitBox& otherHitBox),  
		void(*exitCallback)(const HitBox& thisHitBox, const HitBox& otherHitBox), HitBoxType tag=HitBoxType::None);
	HitBox(float leftBound, float rightBound, float upperBound, float lowerBound, HitBoxType tag = HitBoxType::None);

	// -- Static functions --
	// Check collision for different shapes
	static bool BoxCollisionCheck(const HitBox& thisHitBox, const HitBox& otherHitBox);
	static bool UpRampCollisionCheck(const HitBox& thisHitBox, const HitBox& otherHitBox);
	static bool DownRampCollisionCheck(const HitBox& thisHitBox, const HitBox& otherHitBox);
	// Resolves collisions for different shapes
	static glm::vec2 ResolveBoxX(const HitBox& thisHitBox, const HitBox& otherHitBox);
	static glm::vec2 ResolveUpRampX(const HitBox& thisHitBox, const HitBox& otherHitBox);
	static glm::vec2 ResolveUpRampY(const HitBox& thisHitBox, const HitBox& otherHitBox);
	static glm::vec2 ResolveDownRampX(const HitBox& thisHitBox, const HitBox& otherHitBox);
	static glm::vec2 ResolveDownRampY(const HitBox& thisHitBox, const HitBox& otherHitBox);

	// --Basic Functions--
	// Defines collision checking behavior (is this a box or a ramp?)
	bool(*collisionCheck)(const HitBox& thisHitBox, const HitBox& otherHitBox) = BoxCollisionCheck;
	// Checks collision with another hitbox using collision check logic specified and calls parent entity callback
	bool CheckCollision(const HitBox& other);
	// Checks if hitbox contains point
	bool Contains(glm::vec2 point);
	// (DEBUGGING ONLY, performs TERRIBLY so NEVER call outside debuggin purposes)
	void Render(Renderer* renderer);

	// Gets the position in relation to the world
	glm::vec2 GetGlobalPosition() const;


	// --Variables--
	bool active = true;
	int id = -1;
	// Of all occupied grid cells, it stores top left and bottom right (u can figure out rest from that)
	int topLeftCell = -1;
	int bottomRightCell = -1;
	// Stores the position in relation to the parent entity
	Transform localTransform;
	// Either or both could be null, parentTransform is for more lightweight use
	Entity* parentEntity; 
	Transform* parentTransform;
	HitBoxType tag;
	// Calls function on a collision
	void(*collisionCallback)(const HitBox& thisHitBox, const HitBox& otherHitBox);
	// Calls when they stop colliding
	void(*collisionExitCallback)(const HitBox& thisHitBox, const HitBox& otherHitBox);

	// --Specified Getters--
	// Corners
	glm::vec2 TopLeft() const { return GetGlobalPosition() + glm::vec2(-localTransform.GetScale().x / 2.0f, -localTransform.GetScale().y / 2.0f); }
	glm::vec2 TopRight() const { return GetGlobalPosition() + glm::vec2(localTransform.GetScale().x / 2.0f, -localTransform.GetScale().y / 2.0f); }
	glm::vec2 BottomLeft() const { return GetGlobalPosition() + glm::vec2(-localTransform.GetScale().x / 2.0f, localTransform.GetScale().y / 2.0f); }
	glm::vec2 BottomRight() const { return GetGlobalPosition() + glm::vec2(localTransform.GetScale().x / 2.0f, localTransform.GetScale().y / 2.0f); }
	// Edges
	float TopBound() const { return GetGlobalPosition().y - (localTransform.GetScale().y / 2.0f); }
	float BottomBound() const { return GetGlobalPosition().y + (localTransform.GetScale().y / 2.0f); }
	float LeftBound() const { return GetGlobalPosition().x - (localTransform.GetScale().x / 2.0f); }
	float RightBound()const { return GetGlobalPosition().x + (localTransform.GetScale().x / 2.0f); }

};

