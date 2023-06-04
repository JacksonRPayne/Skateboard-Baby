#include "CollisionGrid.h"

CollisionGrid::CollisionGrid(float cellSize) 
	: CELL_SIZE(cellSize), hitboxes(), dynamicHitboxes(){

	cells = nullptr;
	hitboxes.reserve(10);
}

HitBox* CollisionGrid::Register(const HitBox &hitBox) {
	// Hitbox falls outside of current range of grid
	//if(hitBox.LeftBound() < leftBound || hitBox.RightBound() > rightBound
	//	|| hitBox.TopBound() < topBound || hitBox.BottomBound() > bottomBound) {
	//	ExpandBoundaries(hitBox);
	//}

	hitboxes.emplace_back(hitBox);
	// Index to retrieve this hitbox
	int hitBoxId = hitboxes.size() - 1;
	hitboxes[hitBoxId].id = hitBoxId;
	

	// So basically if this hitbox is dynamic, it will add it to a list of the dynamic hitboxes
	// It knows its dynamic by checking the "dynamic" member of the hitbox
	// If confused email jacksonpayne909@gmail.com with questions
	if (hitBox.dynamic) dynamicHitboxes.push_back(hitBoxId);

	// TODO: uh oh.. vector resize = stale pointer, fuck STL
	return &hitboxes[hitBoxId];

}

void CollisionGrid::ConstructGrid() {
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

	// Insert each hitbox into the grid
	for (int i = 0; i < hitboxes.size(); i++) {
		InsertToGrid(hitboxes[i]);
	}
}

int CollisionGrid::GetCellOfPoint(glm::vec2 point) {
	//if (!gridBounds.Contains(point)) return -1;
	int cell = (int)std::floor((point.x - leftBound) / CELL_SIZE) + (int)std::floor((point.y - topBound) / CELL_SIZE) * cellsXCount;
	if (cell <0 || cell > cellCount - 1) return -1;
	return cell;
}

int CollisionGrid::CellX(int cell) {
	return cell % cellsXCount;
}

int CollisionGrid::CellY(int cell) {
	return (cell / cellsXCount);
}

void CollisionGrid::InsertToGrid(const HitBox& hitbox) {
	//int cellIdx[4];
	//GetCellsOfBox(hitbox, cellIdx);
	//for (int i = 0; i < 4; i++) {
	//	if (cellIdx[i] == -1)continue;
	//	cells[cellIdx[i]].push_back(hitbox.id);
	//}
	//// Sets hitbox cells 
	//memcpy(hitboxes[hitbox.id].cells, cellIdx, 4 * sizeof(int));

	int topLeftCell = GetCellOfPoint(hitbox.TopLeft());
	int bottomRightCell = GetCellOfPoint(hitbox.BottomRight());
	// Out of bounds
	if (topLeftCell == -1 || bottomRightCell == -1) return;

	int cellWidth = CellX(bottomRightCell) - CellX(topLeftCell);
	int cellHeight = CellY(bottomRightCell) - CellY(topLeftCell);

	for (int y = 0; y <= cellHeight; y++) {
		for (int x = 0; x <= cellWidth; x++) {
			int currCell = (y * cellsXCount + x) + topLeftCell;
			cells[currCell].push_back(hitbox.id);
		}
	}

	// Set cell values for hitbox
	hitboxes[hitbox.id].topLeftCell = topLeftCell;
	hitboxes[hitbox.id].bottomRightCell = bottomRightCell;
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
	//for (int i = 0; i < 4; i++) {
	//	int cell = hitbox->cells[i];
	//	if (cell == -1) continue;
	//	// Check collision on each hitbox in that cell
	//	for (int j = 0; j < cells[cell].size(); j++) {
	//		//								 Looks so ugly
	//		hitbox->CheckCollision(hitboxes[(cells[cell])[j]]);
	//	}
	//}
	int topLeftCell = hitbox->topLeftCell;
	int bottomRightCell = hitbox->bottomRightCell;

	int cellWidth = CellX(bottomRightCell) - CellX(topLeftCell);
	int cellHeight = CellY(bottomRightCell) - CellY(topLeftCell);
	// Stores already checked hitbox ids
	// Another way to do it could be to make a set of hbids and construct it in the nested loop
	// then check collisions after. Set overhead might make it slower for this simple of a prob (small n)
	std::vector<int> checkedHitboxes = std::vector<int>();

	for (int y = 0; y <= cellHeight; y++) {
		for (int x = 0; x <= cellWidth; x++) {
			int currCell = (y * cellsXCount + x) + topLeftCell;
			for (int i = 0; i < cells[currCell].size(); i++) { // some nasty nesting
				int hbid = (cells[currCell])[i];
				// Already checked coll on this hb
				if (std::find(checkedHitboxes.begin(), checkedHitboxes.end(), hbid) != checkedHitboxes.end()) continue; 
				hitbox->CheckCollision(hitboxes[hbid]);
				checkedHitboxes.push_back(hbid);
			}
		}
	}
}

void CollisionGrid::UpdateGridPosition(int hitboxId) {

	//int* oldCells = hitboxes[hitboxId].cells;
	//int newCells[4];
	//GetCellsOfBox(hitboxes[hitboxId], newCells);
	//
	//// NOTE: I think this is probably more efficient than checking to see if its changed for value
	//for (int i = 0; i < 4; i++) {	
	//	// Removes hitbox from all cells it was in
	//	cells[oldCells[i]].erase(std::remove(cells[oldCells[i]].begin(), cells[oldCells[i]].end(), hitboxId));
	//	// Adds hitbox to all cells its in now
	//	cells[newCells[i]].push_back(hitboxId);
	//}

	int topLeftCell = GetCellOfPoint(hitboxes[hitboxId].TopLeft());
	int bottomRightCell = GetCellOfPoint(hitboxes[hitboxId].BottomRight());
	// Return if it still occupies the same 
	if (topLeftCell == hitboxes[hitboxId].topLeftCell && bottomRightCell == hitboxes[hitboxId].bottomRightCell) return;
	
	topLeftCell = hitboxes[hitboxId].topLeftCell;
	bottomRightCell = hitboxes[hitboxId].bottomRightCell;

	int cellWidth = CellX(bottomRightCell) - CellX(topLeftCell);
	int cellHeight = CellY(bottomRightCell) - CellY(topLeftCell);
	// Erases this hitbox from all cells it used to be in
	for (int y = 0; y <= cellHeight; y++) {
		for (int x = 0; x <= cellWidth; x++) {
			int currCell = (y * cellsXCount + x) + topLeftCell;
			cells[currCell].erase(std::remove(cells[currCell].begin(), cells[currCell].end(), hitboxId), cells[currCell].end());
		}
	}

	// Reinserts hitbox into correct grid positions
	InsertToGrid(hitboxes[hitboxId]);

}


void CollisionGrid::DEBUG_RENDER(Renderer* renderer) {
	// Outline grid
	renderer->DrawLine(glm::vec2(leftBound, topBound), glm::vec2(rightBound, topBound), 2.0f, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	renderer->DrawLine(glm::vec2(leftBound, bottomBound), glm::vec2(rightBound, bottomBound), 2.0f, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	renderer->DrawLine(glm::vec2(leftBound, topBound), glm::vec2(leftBound, bottomBound), 2.0f, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	renderer->DrawLine(glm::vec2(rightBound, topBound), glm::vec2(rightBound, bottomBound), 2.0f, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

	int cellsX = (rightBound - leftBound) / CELL_SIZE;
	int cellsY = (bottomBound - topBound) / CELL_SIZE;

	for (int i = 1; i < cellsX; i++) {
		float x = leftBound + CELL_SIZE * i;
		renderer->DrawLine(glm::vec2(x, topBound), glm::vec2(x, bottomBound), 2.0f, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
	}
	for (int i = 1; i < cellsY; i++) {
		float y = topBound + CELL_SIZE * i;
		renderer->DrawLine(glm::vec2(leftBound, y), glm::vec2(rightBound, y), 2.0f, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
	}
	for (int i = 0; i < cellCount; i++) {
		if (cells[i].size() == 0) continue;
		glm::vec2 pos = glm::vec2(leftBound + CellX(i)*CELL_SIZE + CELL_SIZE / 2.0f, topBound + CellY(i)*CELL_SIZE + CELL_SIZE / 2.0f);
		glm::vec4 color = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
		switch (cells[i].size()) {
		case 1:
			color = glm::vec4(0.0f, 1.0f, 0.0f, 0.3f);
			break;
		case 2: 
			color = glm::vec4(0.0f, 0.0f, 1.0f, 0.3f);
			break;
		case 3:
			color = glm::vec4(1.0f, 0.0f, 0.0f, 0.3f);
			break;
		}
		renderer->DrawQuad(color, pos, glm::vec2(CELL_SIZE));
	}
}



void CollisionGrid::ExpandBoundaries(const HitBox& hitbox) {
	// Do I actually want this functionality...?
}
