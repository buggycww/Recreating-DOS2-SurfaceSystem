#include "player.h"

#include "collision detection/AABB.h"
#include "collision detection/CollisionWorld.h"
#include "surface/ISurfaceInteractable.h"

#include <algorithm>
#include <cmath>

namespace
{
	AABB BoundsAt(const vec3::Vector3& position,
				  const vec3::Vector3& size,
				  const vec3::Vector3& offset)
	{
		return {
			position.x + offset.x - size.x * 0.5f,
			position.y + offset.y - size.y * 0.5f,
			position.z + offset.z - size.z * 0.5f,
			size.x,
			size.y,
			size.z
		};
	}

	// The collision system answers what the ray hit; the interface answers
	// whether that object is a surface the player may move to.
	bool TryGetSurfaceHit(const CollisionWorld& world,
						  const Ray& ray,
						  RayHit& outHit,
						  ISurfaceInteractable*& outSurface)
	{
		if (!world.raycast(ray, 1000.f, outHit)) return false;

		outSurface = dynamic_cast<ISurfaceInteractable*>(outHit.object);
		return outSurface != nullptr;
	}
}

void Player::Update(float deltaTime)
{
	bHasSurfaceTarget = false;
	ISurfaceInteractable* hoveredSurface = nullptr;
	RayHit hoverHit{};

	if (cam != nullptr && collision != nullptr)
	{
		const Ray ray = GetScreenToWorldRay(GetMousePosition(), *cam);
		TryGetSurfaceHit(*collision, ray, hoverHit, hoveredSurface);
	}

	if (hoveredSurface != nullptr)
	{
		const float radius = arcEndPoint.transform.scale.y * 0.5f;

		bHasSurfaceTarget = true;
		arcEndPoint.transform.position = {
			hoverHit.point.x + hoverHit.normal.x * radius,
			hoverHit.point.y + hoverHit.normal.y * radius,
			hoverHit.point.z + hoverHit.normal.z * radius
		};

		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			hoveredSurface->Interact();

			const vec3::Vector3 surfacePoint = hoveredSurface->GetMoveTarget();
			const float standOffset = colliderSize.y * 0.5f - colliderOffset.y;
			// Small clearance keeps floating-point error from treating the
			// player's feet as intersecting the floor tile.
			constexpr float standClearance = 0.005f;

			moveDestination = {
				surfacePoint.x,
				surfacePoint.y + standOffset + standClearance,
				surfacePoint.z
			};
			bHasMoveTarget = true;

			moveTargetPoint.transform.position = {
				surfacePoint.x,
				surfacePoint.y + moveTargetPoint.transform.scale.y * 0.5f,
				surfacePoint.z
			};
		}
	}

	if (collision != nullptr && bHasMoveTarget)
	{
		MoveTowardsDestination(deltaTime, *collision);
	}

	arcEndPoint.Update(deltaTime);
	moveTargetPoint.Update(deltaTime);
}

void Player::Draw() const
{
	GameObject::Draw();

	if (bHasSurfaceTarget)
	{
		arcEndPoint.Draw();
	}

	if (bHasMoveTarget)
	{
		moveTargetPoint.Draw();
	}
}

void Player::MoveWithCollision(const vec3::Vector3& delta, const CollisionWorld& world)
{
	const float length = std::sqrt(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);
	if (length <= 0.f) return;

	// Sub-step long frames so a fast player cannot tunnel through geometry.
	constexpr float maxStep = 0.25f;
	const int steps = std::max(1, static_cast<int>(std::ceil(length / maxStep)));
	const vec3::Vector3 step = delta * (1.f / static_cast<float>(steps));

	for (int i = 0; i < steps; ++i)
	{
		// Resolve one axis at a time so hitting a wall still allows sliding
		// along it.
		MoveAxis(0, step.x, world);
		MoveAxis(2, step.z, world);
		MoveAxis(1, step.y, world);
	}
}

void Player::MoveAxis(int axis, float amount, const CollisionWorld& world)
{
	if (amount == 0.f) return;

	vec3::Vector3 next = transform.position;

	switch (axis)
	{
	case 0: next.x += amount; break;
	case 1: next.y += amount; break;
	default: next.z += amount; break;
	}

	const AABB bounds = BoundsAt(next, colliderSize, colliderOffset);
	if (world.overlaps(bounds)) return;

	transform.position = next;
}

void Player::MoveTowardsDestination(float deltaTime, const CollisionWorld& world)
{
	vec3::Vector3 toTarget = moveDestination - transform.position;
	const float distance = std::sqrt(toTarget.x * toTarget.x + toTarget.y * toTarget.y + toTarget.z * toTarget.z);

	if (distance <= 0.01f)
	{
		transform.position = moveDestination;
		bHasMoveTarget = false;
		return;
	}

	const float step = std::min(moveSpeed * deltaTime, distance);
	MoveWithCollision(toTarget * (step / distance), world);
}
