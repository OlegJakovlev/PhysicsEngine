#include "ClothGameObject.h"

namespace CustomApplication
{
	ClothGameObject::ClothGameObject(const uint64_t id, const physx::PxTransform& transform) : GameObject(id, GameObject::Type::Cloth, transform)
	{
	}
}
