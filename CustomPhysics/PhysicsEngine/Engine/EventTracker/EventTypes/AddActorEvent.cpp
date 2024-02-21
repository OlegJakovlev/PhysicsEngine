#include "AddActorEvent.h"

namespace PhysicsEngine
{
	AddActorEvent::AddActorEvent(uint64_t id, const Actor* actor) : PhysxEvent::PhysxEvent(id)
	{
		auto actorId = actor->GetActorID();

		const physx::PxActor* physicsActor = actor->GetCurrentPhysxActor();

		auto actorBounds = physicsActor->getWorldBounds();

		// eDISABLE_GRAVITY, eDISABLE_SIMULATION...
		auto actorFlags = physicsActor->getActorFlags();

		// Integer value which corresponds to the concrete type of an object (PxRigidDynamic, PxRigidStatic, Cloth)
		//auto actorConcreteType = physicsActor->getConcreteType();

		// Sphere, plane, capsule, box, convexmesh, trianglemesh, heighfield
		auto actorType = physicsActor->getType();

		/*
		if (const physx::PxRigidDynamic* dynamicRigid = physicsActor->is<physx::PxRigidDynamic>())
		{

		}
		*/
	}

	void AddActorEvent::Apply()
	{
	}

	void AddActorEvent::Invert()
	{
	}

	void AddActorEvent::Serialize()
	{
	}
}
