#include "Foundation.h"
#include <iostream>

bool Foundation::Init()
{
#if PX_PHYSICS_VERSION < 0x304000 // SDK 3.3
	gFoundation = PxCreateFoundation(
		PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
#else
	gFoundation = PxCreateFoundation(
		PX_FOUNDATION_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
#endif

	if (gFoundation == nullptr)
	{
		std::printf("Foundation init failed!\n");
	}

	return gFoundation != nullptr;
}

void Foundation::Release()
{
	gFoundation->release();
	gFoundation = nullptr;
}

const physx::PxFoundation* Foundation::GetFoundationService() const
{
	return gFoundation;
}
