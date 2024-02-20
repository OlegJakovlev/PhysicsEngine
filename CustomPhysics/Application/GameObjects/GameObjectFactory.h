#pragma once

#ifndef GameObjectFactory_H
#define GameObjectFactory_H

#include "GameObject.h"
#include <atomic>
#include <cstdint>
#include "../../PhysicsEngine/Engine/Actors/ActorFactory.h"

class GameObjectFactory
{
private:
	static uint64_t GenerateId();
	static std::atomic<uint64_t> s_lastId;

	ActorFactory* m_actorFactory;

public:
	void Init(ActorFactory* factory);
	GameObject* CreateStaticActor(physx::PxTransform& transform, GameObject::Layer layer);
	GameObject* CreateDynamicActor(physx::PxTransform& transform, GameObject::Layer layer);
	GameObject* CreateKinematicActor(physx::PxTransform& transform, GameObject::Layer layer);
};

#endif