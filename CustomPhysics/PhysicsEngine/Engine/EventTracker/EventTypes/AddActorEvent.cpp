#include "AddActorEvent.h"

namespace PhysicsEngine
{
	AddActorEvent::AddActorEvent(uint64_t id, const Actor* actor) : PhysxEvent(id)
	{
		// TODO: Remove, just for demo purposes
		m_data.actorId = actor->GetActorID();

		/*
		const physx::PxActor* physicsActor = actor->GetCurrentPhysxActor();

		auto actorBounds = physicsActor->getWorldBounds();
		*/
		// eDISABLE_GRAVITY, eDISABLE_SIMULATION...
		// auto actorFlags = physicsActor->getActorFlags();

		// Integer value which corresponds to the concrete type of an object (PxRigidDynamic, PxRigidStatic, Cloth)
		//auto actorConcreteType = physicsActor->getConcreteType();

		// Sphere, plane, capsule, box, convexmesh, trianglemesh, heighfield
		//auto actorType = physicsActor->getType();

		/*
		if (const physx::PxRigidActor* rigidActor = physicsActor->is<physx::PxRigidActor>())
		{
			rigidActor->getGlobalPose();
		}
		*/

		m_size = sizeof(m_data);
	}

	void AddActorEvent::Apply() const
	{
	}

	void AddActorEvent::Invert() const
	{
	}

	void AddActorEvent::Serialize(char* buffer) const
	{
		memcpy(buffer, this, sizeof(*this));
	}
}
