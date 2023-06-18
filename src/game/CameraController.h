#pragma once
#include <vector>
#include "graphics/Transform.h"
#include "physics/HitBox.h"
#include "scene/Camera.h"

struct ParalaxTarget {
	ParalaxTarget(): transform(nullptr), paralaxIntensity(0.0f){}
	ParalaxTarget(Transform* t, float pi) :transform(t), paralaxIntensity(pi){}
	Transform* transform;
	float paralaxIntensity;
};

class CameraController
{
public:
	CameraController():camera(nullptr), followTarget(nullptr){}
	CameraController(Camera* camera);

	void AddParalaxTarget(Transform* transform, float paralaxIntensity) { paralaxTargets.emplace_back(transform, paralaxIntensity); }
	void SetFollowTarget(HitBox* target, float leftBound, float rightBound, float upperBound, float lowerBound);
	void Update(float dt);

	Camera* camera;
	// When target leaves bounds move camera
	HitBox followBounds;
private:
	// Camera will follow this (probably player)
	HitBox* followTarget;

	// Applies paralax effect to these
	std::vector<ParalaxTarget> paralaxTargets;

};

