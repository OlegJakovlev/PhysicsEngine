#pragma once

#ifndef VehicleSDKWrapper_H
#define VehicleSDKWrapper_H

#include "PxPhysicsAPI.h"

namespace PhysicsEngine
{
	class VehicleSDKWrapper
	{
	public:
		bool Init(const physx::PxPhysics* physics);
		void Release();
	};
}


#endif