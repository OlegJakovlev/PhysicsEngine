#include "Actor.h"

Actor::Actor(const uint64_t id, const Type type)
{
	m_type = type;
	m_actorID = id;
}

const uint64_t Actor::GetActorID() const
{
	return m_actorID;
}

const physx::PxActor* Actor::GetPhysicsActor() const
{
	return m_physicsActor;
}

const Actor::Type Actor::GetType() const
{
	return m_type;
}

void Actor::SetCollisionLayer(CollisionFilter::FilterGroup collisionData)
{
	m_collisionData = collisionData;
}

CollisionFilter::FilterGroup Actor::GetCollisionLayer() const
{
	return m_collisionData;
}
