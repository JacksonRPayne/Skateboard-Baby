#include "LevelBuilder.h"

LevelBuilder::LevelBuilder(glm::vec2 startPos, LevelRenderer* levRend, CollisionGrid* grid, Texture* atlas) :
	startPos(startPos), levRend(levRend), grid(grid), atlas(atlas), tileMap()
{
	currPos = startPos;
}


void LevelBuilder::AddGround(SubTexture tile, int count) {
	tileMap.emplace_back(tile, TileType::Ground, count);
	glm::vec2 scale = atlas->SubWorldSize(tile);
	// Adds ground hitbox
	grid->Register(HitBox(currPos.x - scale.x/2, currPos.x + scale.x * count - scale.x/2, currPos.y - scale.y / 2, currPos.y + scale.y / 2, HitBoxType::Ground));

	currPos.x += scale.x * count;
}


void LevelBuilder::AddUpRamp(SubTexture tile, int count) {
	tileMap.emplace_back(tile, TileType::UpRamp, count);
	glm::vec2 scale = atlas->SubWorldSize(tile);
	glm::vec2 totalScale = glm::vec2(scale.x, scale.x) * (float)count;
	grid->Register(HitBox(currPos.x + totalScale.x/2 - scale.x/2, currPos.y - scale.x/2- totalScale.y / 2, totalScale.x, totalScale.y, nullptr, nullptr, nullptr, HitBoxType::UpRamp));

	currPos.x += totalScale.x;
	currPos.y -= totalScale.y;
}

void LevelBuilder::AddDownRamp(SubTexture tile, int count) {
	tileMap.emplace_back(tile, TileType::DownRamp, count);
	glm::vec2 scale = atlas->SubWorldSize(tile);
	glm::vec2 totalScale = glm::vec2(scale.x, scale.x) * (float)count;
	grid->Register(HitBox(currPos.x + totalScale.x / 2 - scale.x / 2, currPos.y - scale.x / 2 + totalScale.y / 2, totalScale.x, totalScale.y, nullptr, nullptr, nullptr, HitBoxType::DownRamp));

	currPos.x += totalScale.x;
	currPos.y += totalScale.y;
}

void LevelBuilder::Build() {
	levRend->AddStep([&](Renderer* rend) {
		if (tileMap.size() == 0) return;
		glm::vec2 pos = startPos;
		
		pos.x -= atlas->SubWorldSize(tileMap[0].subTexture).x;

		for (int i = 0; i < tileMap.size(); i++) {
			switch (tileMap[i].type)
			{
			case TileType::Ground:
			{
				for (int _ = 0; _ < tileMap[i].count; _++) {
					// Gets size of tile from subtexture
					glm::vec2 scale = atlas->SubWorldSize(tileMap[i].subTexture);
					// Moves "turtle"
					pos.x += scale.x;
					rend->DrawQuad(atlas, tileMap[i].subTexture, pos, scale);
				}
				break;
			}
			case TileType::UpRamp:
			{
				for (int _ = 0; _ < tileMap[i].count; _++) {
					pos.y -= 0.25f;
					glm::vec2 scale = atlas->SubWorldSize(tileMap[i].subTexture);
					// Moves "turtle"
					pos.x += scale.x;
					rend->DrawQuad(atlas, tileMap[i].subTexture, pos, scale);
					pos.y -= 0.25f;
				}
				break;
			}

			case TileType::DownRamp:
			{
				for (int _ = 0; _ < tileMap[i].count; _++) {
					pos.y += 0.25f;
					glm::vec2 scale = atlas->SubWorldSize(tileMap[i].subTexture);
					// Moves "turtle"
					pos.x += scale.x;
					rend->DrawQuad(atlas, tileMap[i].subTexture, pos, scale);
					pos.y += 0.25f;
				}
				break;
			}
			}
		}
	});
}