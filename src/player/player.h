#pragma once

#include "structs/gameObjects.h"

class CollisionWorld;

class Player : public GameObject
{
public:
	Player(vec3::Vector3 pos, const Resources& res)
	{
		transform.position = pos;

		colliderSize = { 0.6f, 1.3f, 0.6f };
		colliderOffset = { 0.f, 0.35f, 0.f };

		Transform3D bodyLocal;
		bodyLocal.scale = { .3f, .6f, .3f };

		ModelAttachment& body = Attach(res.cylinderModel.Get(), bodyLocal, DARKGRAY);
		body.outline = true;
		body.outlineColor = rum;

		Transform3D headLocal;
		headLocal.position = { 0.f, .8f, 0.f };
		headLocal.scale = { 0.2f, 0.2f, 0.2f };

		ModelAttachment& head = Attach(res.sphereModel.Get(), headLocal, DARKGRAY);
		head.outline = true;
		head.outlineColor = rum;

		arcEndPoint.transform.scale = { 0.2f };
		arcEndPoint.transform.position = transform.position;
		arcEndPoint.Attach(res.sphereModel.Get(), {}, WHITE);

		moveTargetPoint.transform.scale = { 0.15f };
		moveTargetPoint.transform.position = transform.position;
		moveTargetPoint.Attach(res.sphereModel.Get(), {}, thatch);
	}

	virtual void Update(float deltaTime) override;
	virtual void Draw() const override;

	void SetCamera(Camera3D* camera) { cam = camera; }
	void SetCollisionWorld(const CollisionWorld* world) { collision = world; }

private:
	void MoveWithCollision(const vec3::Vector3& delta, const CollisionWorld& world);
	void MoveAxis(int axis, float amount, const CollisionWorld& world);
	void MoveTowardsDestination(float deltaTime, const CollisionWorld& world);

	const Camera3D* cam = nullptr;
	const CollisionWorld* collision = nullptr;

	bool bHasSurfaceTarget = false;
	bool bHasMoveTarget = false;
	vec3::Vector3 moveDestination{ 0.f };
	float moveSpeed = 5.f;

	GameObject arcEndPoint;
	GameObject moveTargetPoint;
};
