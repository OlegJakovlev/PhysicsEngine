#include "VehicleActor.h"

namespace PhysicsEngine
{
	VehicleActor::VehicleActor(uint64_t id, const VehicleData* vehicleData) : Actor(id, ActorType::Vehicle), m_vehicleData(vehicleData)
	{
	}
	const VehicleData* VehicleActor::GetVehicleData() const
	{
		return m_vehicleData;
	}
}