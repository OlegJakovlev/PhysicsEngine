#include "VehicleSDKWrapper.h"

namespace PhysicsEngine
{
	bool VehicleSDKWrapper::Init(const physx::PxPhysics* physics)
	{
		physx::PxPhysics* physx = const_cast<physx::PxPhysics*>(physics);
		if (!physx::PxInitVehicleSDK(*physx))
		{
			return false;
		}

		return true;
	}

	void VehicleSDKWrapper::Release()
	{
		physx::PxCloseVehicleSDK();
	}
}
