#pragma once

#ifndef VehicleSceneQueryResults_H
#define VehicleSceneQueryResults_H

#include "PxPhysicsAPI.h"

// Adapted from PhysX_3.4\Samples\SampleVehicle\SampleVehicle_WheelQueryResults
class VehicleSceneQueryResults
{
private:
	physx::PxWheelQueryResult* m_wheelQueryResults;
	physx::PxU32 m_maxNumWheels;
	physx::PxU32 m_numWheels;

	VehicleSceneQueryResults();
	~VehicleSceneQueryResults() = default;

public:
	static VehicleSceneQueryResults* Create(const physx::PxU32 maxNumWheels);
	void Release();

	physx::PxWheelQueryResult* AddVehicle(const physx::PxU32 numWheels);
};

#endif
