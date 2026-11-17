#pragma once

#include "structs/vec3.h"

// A surface the player can click to interact with. The collision system answers
// "what did the ray hit"; this interface answers "is that object a surface?"
// and "where should the player move to?".
class ISurfaceInteractable
{
public:
	virtual ~ISurfaceInteractable() = default;

	virtual void Interact() = 0;

	// World-space point the player should move to/stand on.
	virtual vec3::Vector3 GetMoveTarget() const = 0;
};
