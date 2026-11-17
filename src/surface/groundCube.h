#pragma once

#include "structs/cube.h"
#include "ISurfaceInteractable.h"

class GroundCube : public Cube, public ISurfaceInteractable
{
public:
	GroundCube(vec3::Vector3 scale, vec3::Vector3 pos, Color color, Color outlineColor, const Resources& res)
		: Cube(scale, pos, color, outlineColor, res)
	{
	}

	virtual void Interact() override;
	virtual vec3::Vector3 GetMoveTarget() const override;
};
