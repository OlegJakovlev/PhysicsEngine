#include "StaticGameObject.h"

StaticGameObject::StaticGameObject(const uint64_t id, physx::PxTransform& transform) : GameObject(id, GameObject::Type::Static, transform)
{
}
