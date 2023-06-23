#pragma once
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include "graphics/Transform.h"
// Defines the scale of the world coordinates (y positions = -WORLD_SCALE to WORLD_SCALE)
#define WORLD_SCALE 1.0f

class Camera
{
public:
	Camera() {};
	// Takes in window width and height
	Camera(int width, int height);
	// for now return identity
	glm::mat4 GetViewMatrix() { return transform.GetViewMatrix(); }
	// hard coded for now
	const glm::mat4& GetProjectionMatrix() { return projectionMatrix; }
	// Adjusts projection matrix when window is resized
	void OnResize(int width, int height);
	void AdjustProjection(float aspectRatio);

	float RightBound() { return transform.position.x + transform.scale.x * right; }
	float LeftBound() { return transform.position.x - (transform.scale.x * right); }
	float TopBound() { return transform.position.y - transform.scale.y * bottom; }
	float BottomBound() { return transform.position.y + transform.scale.y * bottom;}

	float left = 0;
	float right = 0;
	float bottom = 0;
	float top = 0;

	float aspectRatio = 0;

	int windowWidth = 0;
	int windowHeight = 0;

	Transform transform;
	glm::mat4 projectionMatrix;

};

