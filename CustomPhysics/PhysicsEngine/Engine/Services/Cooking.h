#pragma once

#ifndef Cooking_H
#define Cooking_H

#include "PxPhysicsAPI.h"

namespace PhysicsEngine
{
	class Cooking
	{
	private:
		physx::PxCooking* gCooking;

	public:
		bool Init(const physx::PxFoundation* foundation, const physx::PxPhysics* physics);
		void Release();
	};
}

#endif

