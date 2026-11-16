#pragma once

#include "vec3.h"
#include <vector>
#include "gameObjects.h"
#include "resources.h"

struct Grid
{
public:
	Grid(vec3::Vector3 gridSize, float cellSize, float cellPadding, const Resources& res);

	void Draw();
	void SetOutline(bool enabled);

	vec3::Vector3 GetGridSize() const { return gridSize; }
	float GetCellSize() const { return cellSize; }

private:
	vec3::Vector3 gridSize;
	float cellSize;
	std::vector<Cube> floor;
};