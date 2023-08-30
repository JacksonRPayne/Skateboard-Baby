#pragma once
#include "LevelRenderer.h"
#include "physics/CollisionGrid.h"

enum class TileType {
	Ground,
	UpRamp,
	DownRamp
};

// Stores a section of the tiles
struct LevelSection {

	LevelSection(SubTexture subTexture, TileType type, int count) :
		subTexture(subTexture), type(type), count(count){}

	SubTexture subTexture;
	TileType type;
	int count;
};


// A (possibly) temporary class to make it easier to build and test level designs
// TODO: THIS IS SOOOOOO SLOOWWWWWWWWWWWWWWWWWWWWWWWWWWWw
// must be the switch statement, makes sense.
// EDIT: uh its not slow anymore.... huh... is it bc i changed collisiongrid vector??
// EDIT: thanks for the gold kind stranger :3
class LevelBuilder
{
public: 
	LevelBuilder() : levRend(nullptr), grid(nullptr), atlas(nullptr), startPos(), currPos(), tileMap(){}
	LevelBuilder(glm::vec2 startPos, LevelRenderer* levRend, CollisionGrid* grid, Texture* atlas);

	// Adding tiles (constructing the ground)
	void AddGround(SubTexture tile, int count);
	void AddUpRamp(SubTexture tile, int count);
	void AddDownRamp(SubTexture tile, int count);

	// Finalizes all changes, must be called at the end of adding tiles
	void Build();

private:

	std::vector<LevelSection> tileMap;

	LevelRenderer* levRend;
	CollisionGrid* grid;
	Texture* atlas;

	glm::vec2 startPos;
	// Keeps track of position for hitbox placement
	glm::vec2 currPos;
};

