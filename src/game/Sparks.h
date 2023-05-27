#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "graphics/Renderer.h"

// Wow... a lot of constants
// I should have just made a proper particle system from this. Maybe later
#define MAX_SPARKS 25
#define X_SPEED 6.0f
#define Y_SPEED 6.0f
#define MAX_SLOPE 0.8f
#define MIN_SLOPE -0.5
#define START_SIZE 0.025f
#define SHRINK_SPEED_SUBTLE 0.23f
#define SHRINK_SPEED_INTENSE 0.19f
#define SPAWN_TIME_SUBTLE 0.1f
#define SPAWN_TIME_INTENSE 0.008f
#define RESET_TIME 3.0f

struct Spark {
	Spark(): spawnTime(-1.0f), coords(glm::vec4(0.0f)), m(0.0f){}
	Spark(float st, glm::vec4 c, float slope):spawnTime(st), coords(c), m(slope){}
	float spawnTime;
	// x and y are position and z and w are scale
	glm::vec4 coords;
	// Slope of its trajectory
	float m;
};

struct Sparks {
	Sparks();
	void GenerateSparks(glm::vec2 position, float direction, Renderer* renderer, float intensity=1.0f);
	Spark sparks[MAX_SPARKS];
	int sparkCounter = 0;
	float timer = 0.0f;
	float lastSpawn = -100.0f;
};