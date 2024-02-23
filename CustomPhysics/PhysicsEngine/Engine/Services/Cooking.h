#pragma once

#ifndef Cooking_H
#define Cooking_H

#include "PxPhysicsAPI.h"

namespace PhysicsEngine
{
	class Cooking
	{
	private:
		physx::PxCooking* m_cooking;

	public:
		bool Init(const physx::PxFoundation* foundation, const physx::PxPhysics* physics);
		void Release();

		const physx::PxCooking* GetCooking() const;
	};
}

#endif

