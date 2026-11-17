#include "groundCube.h"

void GroundCube::Interact()
{
}

vec3::Vector3 GroundCube::GetMoveTarget() const
{
	return {
		transform.position.x,
		transform.position.y + transform.scale.y * 0.5f,
		transform.position.z
	};
}
