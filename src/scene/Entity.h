#pragma once
#include "graphics/Renderer.h"
#include "graphics/Transform.h"
#include "graphics/Texture.h"


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

