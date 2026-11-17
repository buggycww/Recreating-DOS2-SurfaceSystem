#pragma once

#include "structs/gameObjects.h"

// A plain visible cube. GroundCube derives from this and adds surface
// interaction; other cubes stay non-interactable by default.
class Cube : public GameObject
{
public:
	Cube(vec3::Vector3 scale, vec3::Vector3 pos, Color color, Color outlineColor, const Resources& res)
	{
		transform.position = pos;
		transform.scale = scale;

		ModelAttachment& body = Attach(res.cubeModel.Get(), {}, color);
		body.outline = true;
		body.outlineColor = outlineColor;
		body.outlineStyle = OutlineStyle::Edges;
	}
};
