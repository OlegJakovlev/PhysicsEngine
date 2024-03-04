#pragma once

#ifndef GameObjectFactory_H
#define GameObjectFactory_H

#include "GameObject.h"
#include <atomic>
#include <cstdint>
#include "../../PhysicsEngine/Engine/Actors/ActorFactory.h"

namespace CustomApplication
{
	class GameObjectFactory
	{
	private:
		static uint64_t GenerateId();
		static std::atomic<uint64_t> s_lastId;

		PhysicsEngine::ActorFactory* m_actorFactory;

	public:
		void Init(PhysicsEngine::ActorFactory* factory);
		GameObject* CreateStaticGameObject(physx::PxTransform& transform, GameObject::Layer layer);
		GameObject* CreateDynamicGameObject(physx::PxTransform& transform, GameObject::Layer layer);
		GameObject* CreateKinematicGameObject(physx::PxTransform& transform, GameObject::Layer layer);
		GameObject* CreateClothGameObject(physx::PxTransform& transform, GameObject::Layer layer);
	};
}

#endif