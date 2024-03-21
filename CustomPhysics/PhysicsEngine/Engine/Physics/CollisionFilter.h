#pragma once

#ifndef CollisionFilter_H
#define CollisionFilter_H

#include "PxPhysicsAPI.h"
#include <unordered_map>
#include "../Types/FilterGroup.h"

namespace PhysicsEngine
{
	class CollisionFilter
	{
	public:
		static const uint8_t k_maxLayers = 32;
		static const std::unordered_map<FilterGroup, FilterNumericGroup> k_groupToIndex;

	private:
		physx::PxSimulationFilterShader m_collisionFilter;

		uint32_t m_collisionMatrix[k_maxLayers];

		static physx::PxFilterFlags CustomFilterShader(physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
													   physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1,
													   physx::PxPairFlags& pairFlags, const void* constantBlock, physx::PxU32 constantBlockSize);

	public:
		static const FilterNumericGroup GetCollisionIndex(FilterGroup group);

		void Init();
		
		physx::PxSimulationFilterShader GetSimFilter() const;

		uint32_t GetCollisionMask(FilterNumericGroup layer) const;
		void SetCollisionMask(FilterNumericGroup layer, uint32_t newMask);
	};
}

#endif