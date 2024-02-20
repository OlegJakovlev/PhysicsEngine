#pragma once

#ifndef DynamicGameObject_H
#define DynamicGameObject_H

#include "GameObject.h"

class DynamicGameObject : public GameObject
{
	friend class GameObjectFactory;

protected:
	DynamicGameObject() = delete;
	DynamicGameObject(const uint64_t id, physx::PxTransform& transform);
};

#endif