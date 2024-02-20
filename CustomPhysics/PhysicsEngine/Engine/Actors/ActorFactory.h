#pragma once

#ifndef ActorFactory_H
#define ActorFactory_H

#include <atomic>
#include "Actor.h"

class ActorFactory
{
private:
	static uint64_t GenerateId();
	static std::atomic<uint64_t> s_lastId;
	physx::PxPhysics* m_physics;

public:
	bool Init(const physx::PxPhysics*);

	// TODO: API Exposed
	Actor* CreateStaticActor(const physx::PxTransform& transform);
	Actor* CreateDynamicActor(const physx::PxTransform& transform);
	Actor* CreateKinematicActor(const physx::PxTransform& transform);
};

#endif