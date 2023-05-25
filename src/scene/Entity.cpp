#include "Entity.h"

Entity::Entity() : transform(Transform()){}

Entity::Entity(float xPos, float yPos, float xScale, float yScale, float rotation, const std::string& name)
	: transform(Transform(xPos, yPos, xScale, yScale, rotation)), name(name) {}
