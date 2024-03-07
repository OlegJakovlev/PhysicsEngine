#pragma once

#ifndef ClothActor_H
#define ClothActor_H

#include "Actor.h"

namespace PhysicsEngine
{
	class ClothActor : public Actor
	{
		friend class ActorFactory;

	private:
		physx::PxClothMeshDesc meshDesc;

		ClothActor(uint64_t id);

	public:
		const physx::PxClothMeshDesc& GetMeshDesc() const;
	};
}

#endif