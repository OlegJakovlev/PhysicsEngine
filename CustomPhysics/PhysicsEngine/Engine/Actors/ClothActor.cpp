#include "ClothActor.h"

namespace PhysicsEngine
{
	ClothActor::ClothActor(uint64_t id, const physx::PxVec2& size, const physx::PxVec2& scale) : Actor(id, ActorType::Cloth)
	{
		m_size = size;
		m_scale = scale;
	}

	const physx::PxClothMeshDesc& ClothActor::GetMeshDesc() const
	{
		return m_meshDesc;
	}

	physx::PxVec2 const& ClothActor::GetSize() const
	{
		return m_size;
	}

	physx::PxVec2 const& ClothActor::GetScale() const
	{
		return m_scale;
	}
}