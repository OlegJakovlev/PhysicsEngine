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
		VehicleActor(uint64_t id, const VehicleData* vehicleData);

	public:
		const VehicleData* GetVehicleData() const;
	};
}

#endif