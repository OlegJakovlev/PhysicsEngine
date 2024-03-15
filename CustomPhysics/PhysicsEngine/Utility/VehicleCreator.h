#pragma once

#ifndef VehicleCreator_H
#define VehicleCreator_H

#include "PxPhysicsAPI.h"
#include "../Engine/Types/VehicleData.h"

namespace PhysicsEngine
{
	class VehicleCreator
	{
	private:
		physx::PxPhysics* m_physics;

	public:
		bool Init(const physx::PxPhysics* physics);

		void Create4W(physx::PxRigidDynamic* rigidBodyActor, const VehicleData* configData);
		void CreateNW(physx::PxRigidDynamic* rigidBodyActor, const VehicleData* configData);
		void CreateTank(physx::PxRigidDynamic* rigidBodyActor, const VehicleData* configData);
	};
}

#endif
