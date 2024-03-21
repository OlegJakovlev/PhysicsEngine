#include "VehicleQueryFilter.h"
#include "../Types/SurfaceType.h"

namespace PhysicsEngine
{
	// Layer based scene queries
	// https://docs.nvidia.com/gameworks/content/gameworkslibrary/physx/guide/Manual/Vehicles.html#filtering
	//filterData0 is the vehicle suspension raycast.
	//filterData1 is the shape potentially hit by the raycast.
	physx::PxQueryHitType::Enum VehicleQueryFilter::VehicleFilterShader(
		physx::PxFilterData filterData0,
		physx::PxFilterData filterData1,
		const void* constantBlock, physx::PxU32 constantBlockSize,
		physx::PxHitFlags& queryFlags)
	{
		return (filterData1.word3 & SurfaceType::DRIVABLE_SURFACE) ? physx::PxQueryHitType::eBLOCK : physx::PxQueryHitType::eNONE;
	}

	bool VehicleQueryFilter::Init()
	{
		m_vehicleQueryFilter = VehicleQueryFilter::VehicleFilterShader;
		return true;
	}

	physx::PxBatchQueryPreFilterShader VehicleQueryFilter::GetVehicleQueryFilter() const
	{
		return physx::PxBatchQueryPreFilterShader();
	}
}
