#include "StaticGameObject.h"

namespace CustomApplication
{
	StaticGameObject::StaticGameObject(const uint64_t id, const physx::PxTransform& transform) : GameObject(id, GameObject::Type::Static, transform)
	{
	}
}
