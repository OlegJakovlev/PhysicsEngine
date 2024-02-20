#pragma once

#ifndef Actor_H
#define Actor_H

#include "../../GlobalDefine.h"
#include <cstdint>
#include <PxPhysicsAPI.h>
#include "../CollisionFilter.h"

namespace PhysicsEngine
{
	class Actor
	{
	public:
		enum Type
		{
			Static,
			Dynamic
		};

	protected:
		uint64_t m_actorID;
		physx::PxActor* m_physicsActor;
		Type m_type;
		CollisionFilter::FilterGroup m_collisionData;

		Actor(const uint64_t id, const Type type);

#ifdef DEBUG_MODE
		void* m_engineLinkedObject;
#endif

	public:
		const uint64_t GetActorID() const;
		const physx::PxActor* GetPhysicsActor() const;
		const Type GetType() const;
		CollisionFilter::FilterGroup GetCollisionLayer() const;

		// TODO: API Expose
		void SetCollisionLayer(uint32_t collisionData);
	};
}

#endif