#pragma once

#include <raylib.h>

// Free-fly camera input and state. Keeping this out of Game means Game only
// orchestrates the frame, while the camera owns its own movement bindings.
class FreeCameraController
{
public:
	FreeCameraController();

	// Places the camera at position and derives yaw/pitch from target.
	void Setup(const Vector3& position, const Vector3& target);

	void Update(float deltaTime);

	Camera3D& GetCamera() { return camera; }
	const Camera3D& GetCamera() const { return camera; }

private:
	Vector3 Forward() const;
	Vector3 HorizontalForward() const;
	Vector3 Right() const;
	void UpdateTarget();

	Camera3D camera{};
	float yaw = 0.f;     // radians, 0 = looking toward -Z
	float pitch = 0.f;   // radians, + = looking up

	float moveSpeed = 10.f;   // units per second
	float turnSpeed = 100.f;  // degrees per second
	float zoomSpeed = 2.f;    // world units per wheel notch
};
