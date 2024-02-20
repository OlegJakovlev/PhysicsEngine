#include "ActorFactory.h"
#include "StaticActor.h"
#include "DynamicActor.h"

namespace PhysicsEngine
{
	std::atomic<uint64_t> ActorFactory::s_lastId = 0;

	uint64_t ActorFactory::GenerateId()
	{
		return ++s_lastId;
	}

	bool ActorFactory::Init(const physx::PxPhysics* physics)
	{
		m_physics = const_cast<physx::PxPhysics*>(physics);
		return true;
	}

	Actor* ActorFactory::CreateStaticActor(const physx::PxTransform& transform)
	{
		StaticActor* actor = new StaticActor(GenerateId());
		actor->m_physicsActor = m_physics->createRigidStatic(transform);
		return actor;
	}

	Actor* ActorFactory::CreateDynamicActor(const physx::PxTransform& transform)
	{
		DynamicActor* actor = new DynamicActor(GenerateId());
		actor->m_physicsActor = m_physics->createRigidDynamic(transform);
		return actor;
	}

	Actor* ActorFactory::CreateKinematicActor(const physx::PxTransform& transform)
	{
		DynamicActor* actor = new DynamicActor(GenerateId());
		auto dynamicPhysxActor = m_physics->createRigidDynamic(transform);
		dynamicPhysxActor->setRigidBodyFlags(physx::PxRigidBodyFlag::eKINEMATIC);
		actor->m_physicsActor = dynamicPhysxActor;
		return actor;
	}
}

