#include "CollisionGrid.h"

CollisionGrid::CollisionGrid(int cellSize) : CELL_SIZE(cellSize), hitboxListHeads(){
	// Hardcoded as 4 starting cells for now
	cells = std::vector<int>(4, -1);
	leftBound = (float)-CELL_SIZE;
	rightBound = (float)CELL_SIZE;
	bottomBound = (float)CELL_SIZE;
	topBound = (float)-CELL_SIZE;
}

void CollisionGrid::Register(HitBox* hitBox) {
	// Hitbox falls outside of current range of grid
	if(hitBox->LeftBound() < leftBound || hitBox->RightBound() > rightBound
		|| hitBox->TopBound() < topBound || hitBox->BottomBound() > bottomBound) {
		ExpandBoundaries(hitBox);
	}

	hitboxListHeads.push_back(hitBox);
	int hitBoxInd = hitboxListHeads.size() - 1;

	

}

int CollisionGrid::GetCellOfPoint(float x, float y){
	return 0;
}

void CollisionGrid::UnRegister(HitBox* hitBox) {

}

void CollisionGrid::CheckCollisions(HitBox* hitBox) {

}


void CollisionGrid::ExpandBoundaries(HitBox* hitbox) {

}