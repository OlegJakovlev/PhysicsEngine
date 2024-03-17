#pragma once

#ifndef VehicleData_H
#define VehicleData_H

#include "PxPhysicsAPI.h"
#include "VehicleType.h"

namespace PhysicsEngine
{
	struct WheelData
	{
		const physx::PxF32 wheelMass;
		const physx::PxF32 wheelMOI;
		const physx::PxF32 wheelRadius;
		const physx::PxF32 wheelWidth;
		const physx::PxVec3* wheelCenterActorOffsets;
		const physx::PxVec3& chassisCMOffset;
		const physx::PxF32 chassisMass;
	};

	struct ActorData
	{
		const physx::PxVehicleChassisData& chassisData;
		physx::PxMaterial** wheelMaterials;
		physx::PxConvexMesh** wheelConvexMeshes;
		const physx::PxFilterData& wheelSimFilterData;
		physx::PxMaterial** chassisMaterials;
		physx::PxConvexMesh** chassisConvexMeshes;
		const physx::PxU32 numChassisMeshes;
		const physx::PxFilterData& chassisSimFilterData;
	};

	struct VehicleData
	{
		physx::PxVehicleWheelsSimData* wheelSimData;
		physx::PxVehicleDriveSimData* driveSimData;
		uint32_t drivableWheels;
		uint32_t undrivableWheels;
		WheelData* wheelData;
		ActorData* actorData;
		VehicleType type;
	};
}


#endif