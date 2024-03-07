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
		GameObject* CreateStaticGameObject(const physx::PxTransform& transform, const GameObject::Layer layer);
		GameObject* CreateDynamicGameObject(const physx::PxTransform& transform, const GameObject::Layer layer);
		GameObject* CreateKinematicGameObject(const physx::PxTransform& transform, const GameObject::Layer layer);
		GameObject* CreateClothGameObject(const physx::PxTransform& transform, const physx::PxVec2& size, const physx::PxU32 width, const physx::PxU32 height, const GameObject::Layer layer);
	};
}

#endif