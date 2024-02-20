#pragma once

#ifndef StaticGameObject_H
#define StaticGameObject_H

#include "GameObject.h"

class StaticGameObject : public GameObject
{
	friend class GameObjectFactory;

private:
	StaticGameObject() = delete;
	StaticGameObject(const uint64_t id, physx::PxTransform& transform);
};

#endif