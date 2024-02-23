#include "Foundation.h"
#include <iostream>

namespace PhysicsEngine
{
	bool Foundation::Init()
	{
		gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);

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
}
