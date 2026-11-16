#pragma once

#include "gameObjects.h"

class Player : public GameObject
{
public:
	Player(vec3::Vector3 pos, const Resources& res)
	{
		transform.position = pos;

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
	}
};