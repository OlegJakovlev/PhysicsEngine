#include "VehicleGameObject.h"

namespace CustomApplication
{
	VehicleGameObject::VehicleGameObject(const uint64_t id) : GameObject(id, GameObject::Type::Vehicle)
	{
	}
}