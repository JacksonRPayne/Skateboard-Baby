#include "CollisionGrid.h"

CollisionGrid::CollisionGrid(float cellSize) 
	: CELL_SIZE(cellSize), hitboxes(){

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
	if (point.x < leftBound || point.x > rightBound || point.y < topBound || point.y > bottomBound) return -1;
	int cell = (int)std::floor((point.x - leftBound) / CELL_SIZE) + (int)std::floor((point.y - topBound) / CELL_SIZE) * cellsXCount;
	return cell;
}

int CollisionGrid::CellX(int cell) {
	return cell % cellsXCount;
}

int CollisionGrid::CellY(int cell) {
	return (cell / cellsXCount);
}


void CollisionGrid::GetCellsOfBox(int hitboxId, int topLeftCell, int bottomRightCell, std::vector<int>* returnCells) {
	glm::vec2 topLeft = hitboxes[hitboxId].TopLeft();
	glm::vec2 bottomRight = hitboxes[hitboxId].BottomRight();

	if (topLeftCell == -1 && bottomRightCell == -1) return;

	// Just top left out of bounds
	if (topLeftCell == -1) {
		// Clamps point to grid borders
		glm::vec2 adjustedPoint(std::max(topLeft.x, leftBound), std::max(topLeft.y, topBound));
		topLeftCell = GetCellOfPoint(adjustedPoint);
	}
	// Just bottom right out of bounds
	if (bottomRightCell == -1) {
		glm::vec2 adjustedPoint(std::min(bottomRight.x, rightBound - 0.001f), std::min(bottomRight.y, bottomBound - 0.001f));
		bottomRightCell = GetCellOfPoint(adjustedPoint);
	}

	int cellWidth = std::max(CellX(bottomRightCell) - CellX(topLeftCell), 0);
	int cellHeight = std::max(CellY(bottomRightCell) - CellY(topLeftCell), 0);


	for (int y = 0; y <= cellHeight; y++) {
		for (int x = 0; x <= cellWidth; x++) {
			int currCell = (y * cellsXCount + x) + topLeftCell;
			if (currCell < 0 || currCell >= cellCount) continue;
			returnCells->push_back(currCell);
		}
	}

	// Set cell values for hitbox
	hitboxes[hitboxId].topLeftCell = topLeftCell;
	hitboxes[hitboxId].bottomRightCell = bottomRightCell;
}

void CollisionGrid::InsertToGrid(const HitBox& hitbox) {

	std::vector<int> occupiedCells = std::vector<int>();
	GetCellsOfBox(hitbox.id, GetCellOfPoint(hitbox.TopLeft()), GetCellOfPoint(hitbox.BottomRight()), &occupiedCells);
	for (int i = 0; i < occupiedCells.size(); i++) {
		cells[occupiedCells[i]].push_back(hitbox.id);
	}
}


void CollisionGrid::CheckCollision(HitBox* hitbox) {

	std::vector<int> occupiedCells = std::vector<int>();
	GetCellsOfBox(hitbox->id, hitbox->topLeftCell, hitbox->bottomRightCell, &occupiedCells);
	// Stores already checked hitbox ids
	// Another way to do it could be to make a set of hbids and construct it in the nested loop
	// then check collisions after. Set overhead might make it slower for this simple of a prob (small n)
	std::vector<int> checkedHitboxes = std::vector<int>();
	for (int c = 0; c < occupiedCells.size(); c++) { // <-- c++ ;)
		for (int i = 0; i < cells[occupiedCells[c]].size(); i++) {
			int hbid = (cells[occupiedCells[c]])[i];
			// Already checked coll on this hb
			if (std::find(checkedHitboxes.begin(), checkedHitboxes.end(), hbid) != checkedHitboxes.end()) continue;
			hitbox->CheckCollision(hitboxes[hbid]);
			checkedHitboxes.push_back(hbid);
		}
	}
}

void CollisionGrid::UpdateGridPosition(int hitboxId) {
	int newTl = GetCellOfPoint(hitboxes[hitboxId].TopLeft());
	int newBr = GetCellOfPoint(hitboxes[hitboxId].BottomRight());

	int tl = hitboxes[hitboxId].topLeftCell;
	int br = hitboxes[hitboxId].bottomRightCell;

	// No change in grid pos
	// NOTE: this doesn't cover one special case where top left and bottom right are out
	// of bounds but the middle is still in the grid (bottom left is in the grid)
	if (newTl == tl && newBr == br) return;

	std::vector<int> oldCells = std::vector<int>();
	GetCellsOfBox(hitboxId, tl, br, &oldCells);
	// Removes this hitbox from all old cells
	for (int i = 0; i < oldCells.size(); i++) {
		cells[oldCells[i]].erase(std::remove(cells[oldCells[i]].begin(), cells[oldCells[i]].end(), hitboxId), cells[oldCells[i]].end());
	}

	// Reinserts hitbox into correct cells
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
