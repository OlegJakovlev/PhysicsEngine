#include "Physics.h"

bool Physics::Init(const physx::PxFoundation* foundation,
				   const physx::PxPvd* visualDebugger)
{
	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, const_cast<physx::PxFoundation&>(*foundation),
							   physx::PxTolerancesScale(),
							   true,
							   const_cast<physx::PxPvd*>(visualDebugger));

	if (gPhysics == nullptr)
	{
		std::printf("Physics init failed!\n");
	}

	return gPhysics != nullptr;
}

void Physics::Release()
{
	gPhysics->release();
	gPhysics = nullptr;
}

const physx::PxPhysics* Physics::GetPhysics() const
{
	return gPhysics;
}
