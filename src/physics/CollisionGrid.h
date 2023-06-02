#pragma once
#include <vector>
#include <limits>
#include "HitBox.h"


// NOTES: Thinking about it this is how it should work:
// -- All hitboxes should be *stored* here and entities will have pointers to them
// -- All hitboxes will be marked static/dynamic and the grid will only check for position changes in dynamic hitboxes
// -- RegisterHitbox(yadayada) will register a hitbox to the grid
// -- CheckCollisions(hitboxptrorsomething) will check collisions for one hitbox
// ---- This way only collision boxes that want to be checked will be checked (make sure only one callback is called)
// ---- Should just have a way to check this hitbox with each other hitbox in the grid cell(s) it's in.
// -- InitializeGrid() will create the collision grid (won't change) once all the level hitboxes have been registered.
// ---- The grid will be of a certain size and just stay that way.
// ---- THIS WONT WORK with dynamic hitboxes: must predefine some boundaries. 
// ---- Think: if u had a fixed sized grid with baby and rail it would be tiny
// ---- but wait............ idea........
// ---- baby only has to check collisions with rail bc even if hes out of bounds in the grid, hes out of bounds bc
// ---- there are no hitboxes there................................
// ---- I think original idea could be epic.
// ---- BUT: the register function now becomes tricky. Or does it... wait it doesn't
// ---- wait it does. Kinda hard to insert hitboxes into grid before I know the size of the grid
// ---- and this would require defining the size of the grid AS i add the hitboxes....
// ---- would have to loop through hitboxes and find min/max x and y vals
// ---- could b worth it doe... woudl make it much easier to think abt boundaries
// -- Hitboxes should be able to define their own collision check and render functions in a NONPOLYMORPHIC way
// ---- Member function pointer that can be redirected, but if not set, is set to default functions.
// ---- This way I can easily support non-box geometry without creating some weird system of different hitbox types.
// ---- This part doesn't make as much sense as I thought it would unless the CheckCollision func somehow references
// ---- a function pointer in "other" to determine bounds. <-- this is what should have funcptr not checkcollision

// Usage:
// Register each desired hitbox, then create the grid
class CollisionGrid
{
public:
	CollisionGrid(float cellSize);

	// Registers new hitbox to grid
	HitBox* Register(const HitBox &hitBox);
	void CreateGrid();
	// Returns the cell index of the point given
	int GetCellOfPoint(glm::vec2 point);
	// Returns a length 4 int
	void GetCellsOfBox(const HitBox& hitbox, int* returnArr);
	// For moving/scaling hitboxes
	void UpdateGridPosition(int hitboxId);
	
	void CheckCollision(HitBox* hitbox);

	float leftBound;
	float rightBound;
	float topBound;
	float bottomBound;
	int cellCount=0;
	// How many horizontal vs vertical cells are there
	int cellsXCount = 0;
	int cellsYCount = 0;
	HitBox gridBounds;

	// Indexes into hitboxes
	std::vector<int>* cells;

private:
	// Expands grid if new hitbox is outside of range
	void ExpandBoundaries(const HitBox &hitbox);
	void InsertToGrid(const HitBox& hitbox);

	// Stores all hitboxes in scene
	std::vector<HitBox> hitboxes;
	// Stores dynamic hitboxes: used to check if hitbox needs to be reassigned to diff grid cells
	std::vector<int> dynamicHitboxes;

	const float CELL_SIZE;

};

