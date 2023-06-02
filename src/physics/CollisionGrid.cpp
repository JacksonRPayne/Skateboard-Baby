#include "CollisionGrid.h"

CollisionGrid::CollisionGrid(float cellSize) 
	: CELL_SIZE(cellSize), hitboxes(), dynamicHitboxes(){

	cells = nullptr;
}

HitBox* CollisionGrid::Register(const HitBox &hitBox) {

	hitboxes.push_back(hitBox);
	// Index to retrieve this hitbox
	int hitBoxId = hitboxes.size() - 1;
	hitboxes[hitBoxId].id = hitBoxId;
	

	// So basically if this hitbox is dynamic, it will add it to a list of the dynamic hitboxes
	// It knows its dynamic by checking the "dynamic" member of the hitbox
	// If confused email jacksonpayne909@gmail.com with questions
	if (hitBox.dynamic) dynamicHitboxes.push_back(hitBoxId);

	return &hitboxes[hitBoxId];

}

void CollisionGrid::CreateGrid() {
	// Represent the minimum and maximum boundaries of the grid
	float minX = std::numeric_limits<float>::max();
	float minY = std::numeric_limits<float>::max();
	float maxX = std::numeric_limits<float>::min();
	float maxY = std::numeric_limits<float>::min();
	// Finds bounds based on registered hitboxes
	for (int i = 0; i < hitboxes.size(); i++) {
		if (hitboxes[i].LeftBound() < minX) minX = hitboxes[i].LeftBound();
		if (hitboxes[i].RightBound() > maxX) maxX = hitboxes[i].RightBound();
		if (hitboxes[i].TopBound() < minY) minY = hitboxes[i].TopBound();
		if (hitboxes[i].BottomBound() > maxY) maxY = hitboxes[i].BottomBound();
	}

	// Rounds max to the nearest cell size value: makes the grid bounds defined in numbers divisible by CELL_SIZE
	// Note: it rounds the maxes, this is arbitrary, it could round the mins
	maxX = std::ceil(maxX / CELL_SIZE - minX / CELL_SIZE) * CELL_SIZE + minX;
	maxY = std::ceil(maxY / CELL_SIZE - minY / CELL_SIZE) * CELL_SIZE + minY;

	// Sets up the grid with our new info about bounds
	leftBound = minX;
	rightBound = maxX;
	topBound = minY;
	bottomBound = maxY;
	cellCount = ((rightBound - leftBound) / CELL_SIZE) * ((bottomBound - topBound) / CELL_SIZE);
	cells = new std::vector<int>[cellCount];
	cellsXCount = (int)((rightBound - leftBound) / CELL_SIZE);
	cellsYCount = (int)((bottomBound - topBound) / CELL_SIZE);
	gridBounds = HitBox(leftBound, rightBound, topBound, bottomBound);

	// Insert each hitbox into the grid
	for (int i = 0; i < hitboxes.size(); i++) {
		InsertToGrid(hitboxes[i]);
	}
}

void CollisionGrid::InsertToGrid(const HitBox& hitbox) {
	int cellIdx[4];
	GetCellsOfBox(hitbox, cellIdx);
	for (int i = 0; i < 4; i++) {
		if (cellIdx[i] == -1)continue;
		cells[cellIdx[i]].push_back(hitbox.id);
	}
	// Sets hitbox cells 
	memcpy(hitboxes[hitbox.id].cells, cellIdx, 4 * sizeof(int));
}

int CollisionGrid::GetCellOfPoint(glm::vec2 point) {
	if (!gridBounds.Contains(point)) return -1;
	int cell = (int)std::floor(point.x / CELL_SIZE) + (int)std::floor(point.y /CELL_SIZE)* cellsYCount;
	return cell;
}

void CollisionGrid::GetCellsOfBox(const HitBox& hitbox, int* returnArr) {
	glm::vec2 corners[4] = { hitbox.TopRight(),hitbox.TopRight(), hitbox.BottomLeft(),hitbox.BottomRight() };
	returnArr[0] = -1;
	returnArr[1] = -1;
	returnArr[2] = -1;
	returnArr[3] = -1;
	// How many cells the hitbox has been inserted to already
	int cellCount = 0;
	int currCell = GetCellOfPoint(hitbox.TopLeft());
	if (currCell != -1) {
		returnArr[0] = currCell;
		cellCount++;
	}
	int nextCell = -1;
	for (int i = 0; i < 4; i++) {
		// Loops through and checks each corner and puts hitbox in every cell it touches
		nextCell = GetCellOfPoint(corners[i]);
		if (nextCell != currCell && nextCell !=-1) {
			returnArr[cellCount] = nextCell;
			currCell = nextCell;
			cellCount++;
		}
	}
}

void CollisionGrid::CheckCollision(HitBox* hitbox) {
	// For each potential cell the hitbox is in
	for (int i = 0; i < 4; i++) {
		int cell = hitbox->cells[i];
		if (cell == -1) continue;
		// Check collision on each hitbox in that cell
		for (int j = 0; j < cells[cell].size(); i++) {
			//								 Looks so ugly
			hitbox->CheckCollision(hitboxes[(cells[cell])[i]]);
		}
	}
}


void CollisionGrid::UpdateGridPosition(int hitboxId) {
	// Hitbox falls outside of current range of grid
	//if(hitBox.LeftBound() < leftBound || hitBox.RightBound() > rightBound
	//	|| hitBox.TopBound() < topBound || hitBox.BottomBound() > bottomBound) {
	//	ExpandBoundaries(hitBox);
	//}

	int* oldCells = hitboxes[hitboxId].cells;
	int newCells[4];
	GetCellsOfBox(hitboxes[hitboxId], newCells);

	// NOTE: I think this is probably more efficient than checking to see if its changed for value
	for (int i = 0; i < 4; i++) {	
		// Removes hitbox from all cells it was in
		cells[oldCells[i]].erase(std::remove(cells[oldCells[i]].begin(), cells[oldCells[i]].end(), hitboxId));
		// Adds hitbox to all cells its in now
		cells[newCells[i]].push_back(hitboxId);
	}
}

void CollisionGrid::ExpandBoundaries(const HitBox &hitbox) {
	// Do I actually want this functionality...?
}