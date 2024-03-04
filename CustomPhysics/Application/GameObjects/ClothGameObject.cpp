#include "ClothGameObject.h"

namespace CustomApplication
{
	ClothGameObject::ClothGameObject(const uint64_t id, physx::PxTransform& transform) : GameObject(id, GameObject::Type::Cloth, transform)
	{
	}
}
