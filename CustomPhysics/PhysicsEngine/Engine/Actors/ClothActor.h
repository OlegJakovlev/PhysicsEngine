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
		physx::PxClothMeshDesc m_meshDesc;
		physx::PxVec2 m_size;
		physx::PxVec2 m_scale;

		ClothActor(uint64_t id, const physx::PxVec2& size, const physx::PxVec2& scale);

	public:
		const physx::PxClothMeshDesc& GetMeshDesc() const;
		const physx::PxVec2 const& GetSize() const;
		const physx::PxVec2 const& GetScale() const;
	};
}

#endif