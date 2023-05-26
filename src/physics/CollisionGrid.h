#pragma once
#include <vector>
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
// -- Hitboxes should be able to define their own collision check and render functions in a NONPOLYMORPHIC way
// ---- Member function pointer that can be redirected, but if not set, is set to default functions.
// ---- This way I can easily support non-box geometry without creating some weird system of different hitbox types.
// ---- This part doesn't make as much sense as I thought it would unless the CheckCollision func somehow references
// ---- a function pointer in "other" to determine bounds. <-- this is what should have funcptr not checkcollision

class CollisionGrid
{
public:
	CollisionGrid(int cellSize);

	// Registers new hitbox to grid
	void Register(HitBox* hitBox);
	// Removes hitbox from grid
	void UnRegister(HitBox* hitBox);
	// Queries grid and checks collisions of that hitbox
	void CheckCollisions(HitBox* hitBox);

	float leftBound;
	float rightBound;
	float topBound;
	float bottomBound;

private:
	// Expands grid if new hitbox is outsize of range
	void ExpandBoundaries(HitBox* hitbox);
	// Returns the cell index of the point given
	int GetCellOfPoint(float x, float y);
	// Stores the head of each linked list of hitboxes in each grid cell
	std::vector<HitBox*> hitboxListHeads; // <-- consider moving ownership here? (std::vector<HitBox>) for better cache use
	// Indexes into hitboxListHeads
	std::vector<int> cells;

	const int CELL_SIZE;

};

