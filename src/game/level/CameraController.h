#pragma once
#include <vector>
#include "graphics/Transform.h"
#include "physics/HitBox.h"
#include "scene/Camera.h"

struct ParallaxTarget {
	ParallaxTarget(): transform(nullptr), parallaxIntensity(0.0f){}
	ParallaxTarget(Transform* t, float intensity) :transform(t), parallaxIntensity(intensity){}
	Transform* transform;
	float parallaxIntensity;
};

class CameraController
{
public:
	CameraController():camera(nullptr), followTarget(nullptr), minimumPos(){}
	CameraController(Camera* camera);

	void AddParalaxTarget(Transform* transform, float parallaxIntensity) { paralaxTargets.emplace_back(transform, parallaxIntensity); }
	void SetFollowTarget(Transform* target, float leftBound, float rightBound, float upperBound, float lowerBound);
	void Update(float dt);

	Camera* camera;
	// When target leaves bounds move camera
	HitBox followBounds;
	// Lowest x and y values permitted
	glm::vec2 minimumPos;
private:
	// Camera will follow this (probably player)
	Transform* followTarget;

	// Applies paralax effect to these
	std::vector<ParallaxTarget> paralaxTargets;

};

