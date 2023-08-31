#pragma once
#include "graphics/Renderer.h"
#include "graphics/Transform.h"
#include "graphics/Texture.h"


// This is a much less necessary class than you might think for one called "Entity"
// Right now its basically only used for hitboxes to have a parent entity
class Entity
{
public:

	Entity();
	Entity(float xPos, float yPos, float xScale, float yScale, float rotation, const std::string& name);

	virtual ~Entity() {};

	virtual void Render(Renderer* renderer) {}
	// Overloaded with specific entity behavior
	virtual void Update(float dt) {}


	Transform transform;
	std::string name;

};

