#include "DynamicGameObject.h"

namespace CustomApplication
{
	DynamicGameObject::DynamicGameObject(const uint64_t id, const physx::PxTransform& transform) : GameObject(id, GameObject::Type::Dynamic, transform)
	{
	}
}
