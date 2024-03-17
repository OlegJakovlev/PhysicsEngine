#include "CollisionFilter.h"

namespace PhysicsEngine
{
	const std::unordered_map<FilterGroup, FilterNumericGroup> CollisionFilter::k_groupToIndex =
	{
			{FilterGroup::Layer_Default, FilterNumericGroup::Index_Default},
			{FilterGroup::Layer_1, FilterNumericGroup::Index_1},
			{FilterGroup::Layer_2, FilterNumericGroup::Index_2},
			{FilterGroup::Layer_3, FilterNumericGroup::Index_3},
			{FilterGroup::Layer_4, FilterNumericGroup::Index_4},
			{FilterGroup::Layer_5, FilterNumericGroup::Index_5},
			{FilterGroup::Layer_6, FilterNumericGroup::Index_6},
			{FilterGroup::Layer_7, FilterNumericGroup::Index_7},
			{FilterGroup::Layer_8, FilterNumericGroup::Index_8},
			{FilterGroup::Layer_9, FilterNumericGroup::Index_9},
			{FilterGroup::Layer_10, FilterNumericGroup::Index_10},
			{FilterGroup::Layer_11, FilterNumericGroup::Index_11},
			{FilterGroup::Layer_12, FilterNumericGroup::Index_12},
			{FilterGroup::Layer_13, FilterNumericGroup::Index_13},
			{FilterGroup::Layer_14, FilterNumericGroup::Index_14},
			{FilterGroup::Layer_15, FilterNumericGroup::Index_15},
			{FilterGroup::Layer_16, FilterNumericGroup::Index_16},
			{FilterGroup::Layer_17, FilterNumericGroup::Index_17},
			{FilterGroup::Layer_18, FilterNumericGroup::Index_18},
			{FilterGroup::Layer_19, FilterNumericGroup::Index_19},
			{FilterGroup::Layer_20, FilterNumericGroup::Index_20},
			{FilterGroup::Layer_21, FilterNumericGroup::Index_21},
			{FilterGroup::Layer_22, FilterNumericGroup::Index_22},
			{FilterGroup::Layer_23, FilterNumericGroup::Index_23},
			{FilterGroup::Layer_24, FilterNumericGroup::Index_24},
			{FilterGroup::Layer_25, FilterNumericGroup::Index_25},
			{FilterGroup::Layer_26, FilterNumericGroup::Index_26},
			{FilterGroup::Layer_27, FilterNumericGroup::Index_27},
			{FilterGroup::Layer_28, FilterNumericGroup::Index_28},
			{FilterGroup::Layer_29, FilterNumericGroup::Index_29},
			{FilterGroup::Layer_30, FilterNumericGroup::Index_30},
			{FilterGroup::Layer_31, FilterNumericGroup::Index_31},
	};

	void CollisionFilter::Init()
	{
		m_collisionFilter = CollisionFilter::CustomFilterShader;
	}

	const FilterNumericGroup CollisionFilter::GetCollisionIndex(FilterGroup group)
	{
		auto it = k_groupToIndex.find(group);
		if (it != k_groupToIndex.end())
		{
			return it->second;
		}

		return FilterNumericGroup::Index_Default;
	}

	// https://docs.nvidia.com/gameworks/content/gameworkslibrary/physx/guide/Manual/RigidBodyCollision.html#collision-filtering
	// Responsible for general layer behaviour
	physx::PxFilterFlags CollisionFilter::CustomFilterShader(
		physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
		physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1,
		physx::PxPairFlags& pairFlags, const void* constantBlock, physx::PxU32 constantBlockSize)
	{
		// Triggers
		if (physx::PxFilterObjectIsTrigger(attributes0) || physx::PxFilterObjectIsTrigger(attributes1))
		{
			if ((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1))
			{
				pairFlags = physx::PxPairFlag::eTRIGGER_DEFAULT;
				return physx::PxFilterFlag::eDEFAULT;
			}
			
			return physx::PxFilterFlag::eSUPPRESS;
		}

		// TODO: Specific behaviour for particles?
		// auto objectType = physx::PxGetFilterObjectType(attributes0);
		// auto otherObjectType = physx::PxGetFilterObjectType(attributes1);b

		if ((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1))
		{
			pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT;
			pairFlags |= physx::PxPairFlag::eSOLVE_CONTACT;
			pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_FOUND;
			pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS;
			pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_LOST;
			pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_CCD;
			pairFlags |= physx::PxPairFlag::eNOTIFY_CONTACT_POINTS;
			pairFlags |= physx::PxPairFlag::eDETECT_CCD_CONTACT;
			return physx::PxFilterFlag::eDEFAULT;
		}

		return physx::PxFilterFlag::eSUPPRESS;
	}

	physx::PxSimulationFilterShader CollisionFilter::GetSimFilter() const
	{
		return m_collisionFilter;
	}

	uint32_t CollisionFilter::GetCollisionMask(FilterNumericGroup layer) const
	{
		return m_collisionMatrix[layer];
	}

	void CollisionFilter::SetCollisionMask(FilterNumericGroup layer, uint32_t newMask)
	{
		m_collisionMatrix[layer] = newMask;
	}
}

