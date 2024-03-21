#include "VehicleActor.h"

namespace PhysicsEngine
{
	VehicleActor::VehicleActor(uint64_t id, const VehicleData* vehicleData) : Actor(id, ActorType::Vehicle), m_vehicleData(vehicleData)
	{
	}

	void VehicleActor::SetVehicleDrive(const physx::PxVehicleDrive* vehicleDrive)
	{
		m_vehicleDrive = vehicleDrive;
	}

	void VehicleActor::Release()
	{
		/* TODO:
		switch (m_vehicleData->type)
		{
			case VehicleType::Default4W:
				((physx::PxVehicleDrive4W*) m_vehicleDrive)->free();
				break;

			case VehicleType::Tank:
				((physx::PxVehicleDriveNW*) m_vehicleDrive)->free();
				break;

			case VehicleType::Custom:
				((physx::PxVehicleDriveTank*) m_vehicleDrive)->free();
				break;

			default:
				break;
		}

		m_vehicleData->wheelSimData->free();
		*/

		Actor::Release();
	}

	const VehicleData* VehicleActor::GetVehicleData() const
	{
		return m_vehicleData;
	}

	const physx::PxVehicleDrive* VehicleActor::GetVehicleDrive() const
	{
		return m_vehicleDrive;
	}
}