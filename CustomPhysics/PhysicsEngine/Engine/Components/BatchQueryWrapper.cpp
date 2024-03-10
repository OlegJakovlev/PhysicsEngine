#include "BatchQueryWrapper.h"

bool PhysicsEngine::BatchQueryWrapper::Init(int wheelAmount)
{
	physx::PxRaycastQueryResult* sqResults = new physx::PxRaycastQueryResult[wheelAmount];
	physx::PxRaycastHit* sqHitBuffer = new physx::PxRaycastHit[wheelAmount];
	physx::PxBatchQueryDesc sqDesc = physx::PxBatchQueryDesc(wheelAmount, 0, 0);

	sqDesc.queryMemory.userRaycastResultBuffer = sqResults;
	sqDesc.queryMemory.userRaycastTouchBuffer = sqHitBuffer;
	sqDesc.queryMemory.raycastTouchBufferSize = wheelAmount;
	/*
	sqDesc.preFilterShader = myFilterShader;

	physx::PxBatchQuery* batchQuery = scene->createBatchQuery(sqDesc);

	const PxU32 numWheels = 4;

	physx::PxVehicleWheelsSimData* wheelsSimData = PxVehicleWheelsSimData::allocate(numWheels);
	setupWheelsSimulationData(wheelsSimData);

	physx::PxVehicleDriveSimData4W driveSimData;
	setupDriveSimData(driveSimData);

	physx::PxRigidDynamic* vehActor = myPhysics.createRigidDynamic(startPose);
	setupVehicleActor(vehActor);
	myScene.addActor(*vehActor);

	physx::PxVehicleDrive4W* vehDrive4W = physx::PxVehicleDrive4W::allocate(numWheels);
	vehDrive4W->setup(physics, veh4WActor, *wheelsSimData, driveSimData, numWheels - 4);
	wheelsSimData->free();


	physx::PxVehicleWheels* vehicles[1] = { myVehicle };
	physx::PxVehicleSuspensionRaycasts(batchQuery, 1, vehicles, 4, sqResults);

	physx::PxVehicleUpdates(0.01, m_gravity, frictionPairs, 1, vehicles, NULL);

	*/
	return true;
}
