#pragma once

#ifndef VehicleQueryFilter_H
#define VehicleQueryFilter_H

#include <PxQueryFiltering.h>

namespace PhysicsEngine
{
	class VehicleQueryFilter
	{
	private:
		physx::PxBatchQueryPreFilterShader m_vehicleQueryFilter;

		static physx::PxQueryHitType::Enum VehicleFilterShader(physx::PxFilterData filterData0,
															   physx::PxFilterData filterData1,
															   const void* constantBlock, physx::PxU32 constantBlockSize,
															   physx::PxHitFlags& queryFlags);

	public:
		bool Init();
		physx::PxBatchQueryPreFilterShader GetVehicleQueryFilter() const;
	};
}


#endif