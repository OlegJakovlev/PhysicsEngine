#ifndef ClothGameObject_H
#define ClothGameObject_H

#include "GameObject.h"
#include "../Rendering/ClothRenderData.h"

namespace CustomApplication
{
	class ClothGameObject : public GameObject
	{
		friend class GameObjectFactory;

	public:
		ClothGameObject() = delete;
		ClothGameObject(const uint64_t id, physx::PxTransform& transform);
	};
}

#endif