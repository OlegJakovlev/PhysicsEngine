#pragma once

#ifndef VehicleData_H
#define VehicleData_H

#include "PxPhysicsAPI.h"
#include "VehicleType.h"

namespace PhysicsEngine
{
	struct VehicleData
	{
		physx::PxVehicleWheelsSimData* wheelSimData;
		physx::PxVehicleDriveSimData* driveSimData;
		uint32_t drivableWheels;
		uint32_t undrivableWheels;
		VehicleType type;
		uint32_t _padding;
	};
}


#endif