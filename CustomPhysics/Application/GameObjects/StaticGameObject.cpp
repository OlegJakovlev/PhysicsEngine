#include "StaticGameObject.h"

namespace CustomApplication
{
	StaticGameObject::StaticGameObject(const uint64_t id) : GameObject(id, GameObject::Type::Static)
	{
	}
}
