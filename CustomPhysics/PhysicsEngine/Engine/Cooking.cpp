#include "Cooking.h"

bool Cooking::Init(const physx::PxFoundation* foundation, const physx::PxPhysics* physics)
{
	physx::PxFoundation* foundationObject = const_cast<physx::PxFoundation*>(foundation);

	gCooking = PxCreateCooking(PX_PHYSICS_VERSION, *foundationObject, physx::PxCookingParams(physics->getTolerancesScale()));

	return true;
}

void Cooking::Release()
{
	gCooking->release();
	gCooking = nullptr;
}
