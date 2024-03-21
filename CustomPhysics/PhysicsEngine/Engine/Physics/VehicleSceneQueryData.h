#pragma once

#ifndef VehicleSceneQueryData_H
#define VehicleSceneQueryData_H

#include "PxPhysicsAPI.h"

namespace PhysicsEngine
{
	// Adapted from Samples\SampleVehicle\SampleVehicle_SceneQuery.h
	class VehicleSceneQueryData
	{
	private:
		// One result for each wheel.
		physx::PxRaycastQueryResult* m_sqResults;
		physx::PxU32 m_nbSqResults;

		// One hit for each wheel.
		physx::PxRaycastHit* m_sqHitBuffer;

		//Maximum number of suspension raycasts that can be supported by the allocated buffers 
		//assuming a single query and hit per suspension line.
		physx::PxU32 m_numQueries;

		VehicleSceneQueryData();
		~VehicleSceneQueryData() = default;

	public:
		static VehicleSceneQueryData* Create(const physx::PxU32 maxNumWheels);
		void Release();

		physx::PxBatchQuery* SetUpBatchedSceneQuery(physx::PxScene* physxScene, physx::PxBatchQueryPreFilterShader filter);
	};
}

#endif