#include "VehicleSceneQueryResults.h"

VehicleSceneQueryResults::VehicleSceneQueryResults()
{
	m_wheelQueryResults = nullptr;
	m_maxNumWheels = 0;
	m_numWheels = 0;
}

VehicleSceneQueryResults* VehicleSceneQueryResults::Create(const physx::PxU32 maxNumWheels)
{
	const size_t totalSize = sizeof(VehicleSceneQueryResults) + sizeof(physx::PxWheelQueryResult) * maxNumWheels;
	VehicleSceneQueryResults* result = (VehicleSceneQueryResults*) malloc(totalSize);
	if (result == nullptr)
	{
		return nullptr;
	}

	result->m_maxNumWheels = maxNumWheels;
	result->m_wheelQueryResults = (physx::PxWheelQueryResult*)(result + 1);

	for (uint32_t i = 0; i < maxNumWheels; i++)
	{
		new(&result->m_wheelQueryResults[i]) physx::PxWheelQueryResult();
	}

	return result;
}

void VehicleSceneQueryResults::Release()
{
	free(this);
}

physx::PxWheelQueryResult* VehicleSceneQueryResults::AddVehicle(const physx::PxU32 numWheels)
{
	PX_ASSERT((mNumWheels + numWheels) <= mMaxNumWheels);

	physx::PxWheelQueryResult* r = &m_wheelQueryResults[m_numWheels];
	m_numWheels += numWheels;

	return r;
}


