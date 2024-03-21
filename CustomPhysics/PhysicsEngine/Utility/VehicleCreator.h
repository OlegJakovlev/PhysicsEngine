#pragma once

#ifndef VehicleCreator_H
#define VehicleCreator_H

#include "PxPhysicsAPI.h"
#include "../Engine/Types/VehicleData.h"

namespace PhysicsEngine
{
	// TODO: Replace with builder pattern
	// https://refactoring.guru/design-patterns/builder
	class VehicleCreator
	{
	private:
		physx::PxPhysics* m_physics;
		const VehicleData* m_currentConfigData;

	public:
		bool Init(const physx::PxPhysics* physics);

		void SetupWheelsSimulationData(const VehicleData* configData);
		void SetupDriveSimulationData(const VehicleData* configData);
		void SetupVehicleActor(physx::PxRigidDynamic* rigidBodyActor, const VehicleData* configData);

		physx::PxVehicleDrive* Create4W(physx::PxRigidDynamic* rigidBodyActor, const VehicleData* configData);
		physx::PxVehicleDrive* CreateNW(physx::PxRigidDynamic* rigidBodyActor, const VehicleData* configData);
		physx::PxVehicleDrive* CreateTank(physx::PxRigidDynamic* rigidBodyActor, const VehicleData* configData);
	};
}

#endif
