#include "CollisionFilter.h"

void CollisionFilter::Init()
{
	//m_collisionFilter = physx::PxDefaultSimulationFilterShader;
	m_collisionFilter = CollisionFilter::CustomFilterShader;
}

// https://docs.nvidia.com/gameworks/content/gameworkslibrary/physx/guide/Manual/RigidBodyCollision.html#collision-filtering
// Responsible for general layer behaviour
physx::PxFilterFlags CollisionFilter::CustomFilterShader(
	physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
	physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1,
	physx::PxPairFlags& pairFlags, const void* constantBlock, physx::PxU32 constantBlockSize)
{
	// Ignore trigger collision
	if (physx::PxFilterObjectIsTrigger(attributes0) || physx::PxFilterObjectIsTrigger(attributes1))
	{
		pairFlags = physx::PxPairFlag::eTRIGGER_DEFAULT;
		return physx::PxFilterFlag::eDEFAULT;
	}

	// Generate contacts for all that were not filtered above
	pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT;
	//pairFlags |= physx::PxPairFlag::eNOTIFY_CONTACT_POINTS

	// TODO: Dedicated CCD layer?
	// TODO: Dedicated Particle layer?
	pairFlags |= physx::PxPairFlag::eSOLVE_CONTACT;
	pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS;
	pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_CCD;
	pairFlags |= physx::PxPairFlag::eDETECT_CCD_CONTACT;

	// Trigger the contact callback for pairs (A,B)
	if ((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1))
	{
		pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_FOUND;
		pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_LOST;
		pairFlags |= physx::PxPairFlag::eNOTIFY_CONTACT_POINTS;
		pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS;
	}

	return physx::PxFilterFlag::eDEFAULT;
}

physx::PxSimulationFilterShader CollisionFilter::GetFilter() const
{
	return m_collisionFilter;
}

uint32_t CollisionFilter::GetCollisionMask(int layer) const
{
	return m_collisionMatrix[layer];
}

void CollisionFilter::SetCollisionMask(int layer, uint32_t newMask)
{
	m_collisionMatrix[layer] = newMask;
}
