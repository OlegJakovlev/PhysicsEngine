#pragma once

#ifndef VehicleActor_H
#define VehicleActor_H

#include "Actor.h"
#include "../Types/VehicleData.h"

namespace PhysicsEngine
{
	class VehicleActor : public Actor
	{
		friend class ActorFactory;

	private:
		const VehicleData* m_vehicleData;
		const physx::PxVehicleDrive* m_vehicleDrive;

		VehicleActor(uint64_t id, const VehicleData* vehicleData);
		void SetVehicleDrive(const physx::PxVehicleDrive* vehicleDrive);

	public:
		void Release() override;

		const VehicleData* GetVehicleData() const;
		const physx::PxVehicleDrive* GetVehicleDrive() const;
	};
}

#endif