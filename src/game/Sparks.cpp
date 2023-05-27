#include "Sparks.h"

Sparks::Sparks() : sparks(){
}


glm::vec4 SparkFunction(float x, float m, float intensity) {
	float shrinkSpeed = (1 - intensity) * SHRINK_SPEED_SUBTLE + intensity * SHRINK_SPEED_INTENSE;
	float xPos =  x * X_SPEED;
	float yPos = -m * x * Y_SPEED;
	float xSize = std::max(START_SIZE-x * shrinkSpeed, 0.0f);
	float ySize = std::max(START_SIZE-x * shrinkSpeed, 0.0f);
	return glm::vec4(xPos, yPos, xSize, ySize);
}


void Sparks::GenerateSparks(glm::vec2 position, float direction, Renderer* renderer, float intensity) {
	float time = glfwGetTime();
	if (time - timer > RESET_TIME) {
		// Reset? what does reseting even do
	}
	timer = time;
	// Linearly interpolates between spawn rates
	float spawnTime = (1 - intensity)*SPAWN_TIME_SUBTLE + intensity * SPAWN_TIME_INTENSE;
	// Time to spawn a new spark
	if ((timer-lastSpawn) >= spawnTime) {
		float m = ((MAX_SLOPE - MIN_SLOPE) / RAND_MAX) * (float)std::rand() + MIN_SLOPE;
		sparks[sparkCounter % MAX_SPARKS] = Spark(timer, glm::vec4(position.x, position.y, START_SIZE, START_SIZE), m);
		sparkCounter++;
		lastSpawn = timer;
	}

	for (int i = 0; i < MAX_SPARKS; i++) {
		// For sparks that aren't supposed to render (at beggining of spawning spark basically)
		if (sparks[i].spawnTime < 0) continue;

		sparks[i].coords = SparkFunction(timer - sparks[i].spawnTime, sparks[i].m, intensity);
		glm::vec2 pos = { position.x - direction * sparks[i].coords.x, position.y + sparks[i].coords.y };
		glm::vec2 scale = { sparks[i].coords.z, sparks[i].coords.w };
		renderer->DrawQuad(glm::vec4(scale.x*20.0f, scale.x * 20.0f, scale.x * 20.0f, 1.0f), pos, scale);
	}

}
