#include "AddActorEvent.h"
#include <sstream>

namespace PhysicsEngine
{
	AddActorEvent::AddActorEvent(uint64_t id, const Actor* actor) : PhysxEvent(id)
	{
		m_data.eventType = m_type;

		// TODO: Remove, just for demo purposes
		m_data.actorId = actor->GetActorID();
		m_data.actorType = actor->GetType();

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
		memcpy(buffer, &m_data, sizeof(m_data));
	}

	std::string AddActorEvent::SerializeDebug() const
	{
		std::ostringstream s;

#ifdef PHYSICS_DEBUG_MODE
		s << "<AddActorEvent>\n";
		s << "\t<ID>" << m_data.actorId << "</ID>\n";
		s << "\t<Type>" << m_data.actorType << "</ID>\n";
		s << "</AddActorEvent>\n";
#endif

		return s.str();
	}
}
