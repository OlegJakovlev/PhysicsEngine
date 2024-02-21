#pragma once

#ifndef Actor_H
#define Actor_H

#include "../../GlobalDefine.h"
#include <cstdint>
#include <PxPhysicsAPI.h>
#include "../Physics/CollisionFilter.h"

namespace PhysicsEngine
{
	class Actor
	{
	public:
		enum Type
		{
			Default,
			Static,
			Dynamic
		};

	private:
		void CloneActor(Actor*& clone);
		void CloneShapes(physx::PxRigidActor* rigidActor, Actor* clone);

	protected:
		uint64_t m_actorID;
		physx::PxActor* m_currentPhysxActor;
		void** m_gameEnginePointerToPhysicsActor;

#ifdef PHYSICS_DEBUG_MODE
		Actor** m_gameEnginePointerToPhysicsActorDebug;
#endif

		Type m_type;
		CollisionFilter::FilterGroup m_collisionData;

		Actor(const uint64_t id, const Type type);

	public:
		Actor* CloneToRender();
		Actor* CloneToPhysics();

		void Release();

		const uint64_t GetActorID() const;
		const physx::PxActor* GetCurrentPhysxActor() const;
		const Type GetType() const;
		CollisionFilter::FilterGroup GetCollisionLayer() const;

		// TODO: API Expose
		void SetCollisionLayer(uint32_t collisionData);
		void SetGameEnginePointerToPhysicsActor(void** gameEnginePointerToPhysicsActor);

#ifdef PHYSICS_DEBUG_MODE
		void SetGameEnginePointerToPhysicsActorDebug(Actor** gameEnginePointerToPhysicsActorDebug);
#endif
	};
}

#endif