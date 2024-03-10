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

		physx::PxVehicleSetBasisVectors(physx::PxVec3(0,1,0), physx::PxVec3(0, 0, 1));
		physx::PxVehicleSetUpdateMode(physx::PxVehicleUpdateMode::eACCELERATION);

		return true;
	}

	void VehicleSDKWrapper::Release()
	{
		physx::PxCloseVehicleSDK();
	}
}
