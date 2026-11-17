#include "freeCameraController.h"

#include <algorithm>
#include <cmath>
#include <raymath.h>

FreeCameraController::FreeCameraController()
{
	camera.up = Vector3{ 0.f, 1.f, 0.f };
	camera.fovy = 45.f;
	camera.projection = CAMERA_PERSPECTIVE;
}

void FreeCameraController::Setup(const Vector3& position, const Vector3& target)
{
	camera.position = position;

	const Vector3 toTarget = Vector3Subtract(target, position);
	yaw = std::atan2(toTarget.x, -toTarget.z);
	pitch = std::atan2(toTarget.y, std::sqrt(toTarget.x * toTarget.x + toTarget.z * toTarget.z));

	UpdateTarget();
}

void FreeCameraController::Update(float deltaTime)
{
	// ROTATION =======================================================
	const float turnInput = float(IsKeyDown(KEY_PAGE_DOWN)) - float(IsKeyDown(KEY_PAGE_UP));
	yaw += turnInput * turnSpeed * DEG2RAD * deltaTime;

	const float pitchInput = float(IsKeyDown(KEY_E)) - float(IsKeyDown(KEY_Q));
	pitch = std::clamp(pitch + pitchInput * turnSpeed * DEG2RAD * deltaTime,
					   -85.f * DEG2RAD, 85.f * DEG2RAD);

	// MOVEMENT =======================================================
	const float forwardInput = float(IsKeyDown(KEY_W)) - float(IsKeyDown(KEY_S));
	const float strafeInput = float(IsKeyDown(KEY_D)) - float(IsKeyDown(KEY_A));
	const float verticalInput = float(IsKeyDown(KEY_UP)) - float(IsKeyDown(KEY_DOWN));

	Vector3 move = { 0.f, 0.f, 0.f };

	if (forwardInput != 0.f || strafeInput != 0.f)
	{
		const Vector3 forward = HorizontalForward();
		const Vector3 right = Right();

		move.x = forward.x * forwardInput + right.x * strafeInput;
		move.z = forward.z * forwardInput + right.z * strafeInput;

		// Normalise, so W+D is not ~1.41x faster than W alone.
		const float length = std::sqrt(move.x * move.x + move.z * move.z);
		move.x /= length;
		move.z /= length;
	}

	move.y = verticalInput;

	camera.position = Vector3Add(camera.position, Vector3Scale(move, moveSpeed * deltaTime));

	// ZOOM ===========================================================
	const float wheel = GetMouseWheelMove();
	if (wheel != 0.f)
	{
		camera.position = Vector3Add(
			camera.position,
			Vector3Scale(Forward(), wheel * zoomSpeed)
		);

		// Keep the camera above the floor so pitch-zooming doesn't burrow under it.
		camera.position.y = std::max(camera.position.y, 0.5f);
	}

	UpdateTarget();
}

void FreeCameraController::UpdateTarget()
{
	camera.target = Vector3Add(camera.position, Forward());
}

Vector3 FreeCameraController::Forward() const
{
	const float cosPitch = std::cos(pitch);
	return Vector3{
		std::sin(yaw) * cosPitch,
		std::sin(pitch),
		-std::cos(yaw) * cosPitch
	};
}

Vector3 FreeCameraController::HorizontalForward() const
{
	return Vector3{ std::sin(yaw), 0.f, -std::cos(yaw) };
}

Vector3 FreeCameraController::Right() const
{
	return Vector3{ std::cos(yaw), 0.f, std::sin(yaw) };
}
