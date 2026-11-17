#pragma once

#include "structs/vec3.h"
#include <vector>
#include "structs/resources.h"
#include "groundCube.h"

struct Grid
{
public:
	Grid(vec3::Vector3 gridSize, float cellSize, float cellPadding, const Resources& res);

	void Draw();
	void SetOutline(bool enabled);

	// Every solid piece of level geometry. With click-to-move the player only
	// ever targets floor cells, so no boundary colliders are needed.
	std::vector<GameObject*> GetColliders();

	vec3::Vector3 GetGridSize() const { return gridSize; }
	float GetCellSize() const { return cellSize; }

private:
	vec3::Vector3 gridSize;
	float cellSize;
	std::vector<GroundCube> floor;
};
