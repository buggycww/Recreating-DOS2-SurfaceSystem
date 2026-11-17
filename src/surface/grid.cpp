#include "grid.h"
#include "constants/colors.h"

Grid::Grid(vec3::Vector3 gridSize, float cellSize, float cellPadding, const Resources& res) : gridSize(gridSize), cellSize(cellSize)
{
	vec3::Vector3 offset = { (gridSize.x - 1) * 0.5f * cellSize, -cellSize * 0.5f, (gridSize.z - 1) * 0.5f * cellSize };
	vec3::Vector3 scale = { cellSize - cellPadding };

	for (int rows = 0; rows < gridSize.z; ++rows)
	{
		for (int cols = 0; cols < gridSize.x; ++cols)
		{
			for (int height = 0; height < gridSize.y; ++height)
			{
				vec3::Vector3 pos = vec3::Vector3(cols * cellSize, height * cellSize, rows * cellSize) - offset;
				GroundCube cube = GroundCube(scale, pos, albescentWhite, bouquet, res);
				floor.push_back(cube);
			}
		}
	}
}

void Grid::Draw()
{
	for (const GroundCube& cube : floor)
	{
		cube.Draw();
	}
}

void Grid::SetOutline(bool enabled)
{
	for (GroundCube& cube : floor)
	{
		cube.SetOutline(enabled);
	}
}

std::vector<GameObject*> Grid::GetColliders()
{
	std::vector<GameObject*> colliders;
	colliders.reserve(floor.size());

	for (GroundCube& cube : floor)
	{
		colliders.push_back(&cube);
	}

	return colliders;
}
