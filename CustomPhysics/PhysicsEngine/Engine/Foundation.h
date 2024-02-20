#pragma once

#ifndef Foundation_H
#define Foundation_H

#include "PxPhysicsAPI.h"

namespace PhysicsEngine
{
	class Foundation
	{
	private:
		physx::PxDefaultErrorCallback gDefaultErrorCallback;
		physx::PxDefaultAllocator gDefaultAllocatorCallback;
		physx::PxFoundation* gFoundation;

	public:
		bool Init();
		void Release();
		const physx::PxFoundation* GetFoundationService() const;
	};
}

#endif