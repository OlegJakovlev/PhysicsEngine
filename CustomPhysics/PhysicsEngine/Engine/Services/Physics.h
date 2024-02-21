#pragma once

#ifndef Physics_H
#define Physics_H

#include "PxPhysicsAPI.h"
#include <iostream>

namespace PhysicsEngine
{
	class Physics
	{
	private:
		physx::PxPhysics* gPhysics;

	public:
		bool Init(const physx::PxFoundation* foundation,
				  const physx::PxPvd* visualDebugger);

		void Release();

		const physx::PxPhysics* GetPhysics() const;
	};
}


#endif
