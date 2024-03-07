#pragma once

#ifndef StaticGameObject_H
#define StaticGameObject_H

#include "GameObject.h"

namespace CustomApplication
{
	class StaticGameObject : public GameObject
	{
		friend class GameObjectFactory;

	private:
		StaticGameObject() = delete;
		StaticGameObject(const uint64_t id, const physx::PxTransform& transform);
	};
}

#endif