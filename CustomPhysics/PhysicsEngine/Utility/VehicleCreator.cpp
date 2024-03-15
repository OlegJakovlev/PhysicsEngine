#include "VehicleCreator.h"

namespace PhysicsEngine
{
	bool VehicleCreator::Init(const physx::PxPhysics* physics)
	{
		m_physics = const_cast<physx::PxPhysics*>(physics);

		return true;
	}

	void VehicleCreator::Create4W(physx::PxRigidDynamic* rigidBodyActor, const VehicleData* configData)
	{
		int undrivableWheels = configData->undrivableWheels;

		auto* nonCastVehicleSimData = const_cast<physx::PxVehicleDriveSimData*>(configData->driveSimData);
		auto* vehicleDriveSimData4W = (physx::PxVehicleDriveSimData4W*) nonCastVehicleSimData;

		physx::PxVehicleDrive4W* vehicleDrive4W = physx::PxVehicleDrive4W::allocate(configData->drivableWheels + undrivableWheels);
		vehicleDrive4W->setup(m_physics, rigidBodyActor, *configData->wheelSimData, *vehicleDriveSimData4W, undrivableWheels);
	}

	void VehicleCreator::CreateNW(physx::PxRigidDynamic* rigidBodyActor, const VehicleData* configData)
	{
		int undrivableWheels = configData->undrivableWheels;

		auto* nonConstVehicleSimData = const_cast<physx::PxVehicleDriveSimData*>(configData->driveSimData);
		auto* vehicleDriveSimDataNW = (physx::PxVehicleDriveSimDataNW*) nonConstVehicleSimData;

		physx::PxVehicleDriveNW* vehDriveTank = physx::PxVehicleDriveNW::allocate(configData->drivableWheels + undrivableWheels);
		vehDriveTank->setup(m_physics, rigidBodyActor, *configData->wheelSimData, *vehicleDriveSimDataNW, undrivableWheels);
	}

	void VehicleCreator::CreateTank(physx::PxRigidDynamic* rigidBodyActor, const VehicleData* configData)
	{
		int undrivableWheels = configData->undrivableWheels;

		auto* nonConstVehicleSimData = const_cast<physx::PxVehicleDriveSimData*>(configData->driveSimData);
		auto* vehicleDriveSimDataNW = (physx::PxVehicleDriveSimDataNW*) nonConstVehicleSimData;

		physx::PxVehicleDriveTank* vehDriveTank = physx::PxVehicleDriveTank::allocate(configData->drivableWheels + undrivableWheels);
		vehDriveTank->setup(m_physics, rigidBodyActor, *configData->wheelSimData, *vehicleDriveSimDataNW, undrivableWheels);
	}
}
