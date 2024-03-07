#ifndef ClothGameObject_H
#define ClothGameObject_H

#include "GameObject.h"

namespace CustomApplication
{
	class ClothGameObject : public GameObject
	{
		friend class GameObjectFactory;

	public:
		ClothGameObject() = delete;
		ClothGameObject(const uint64_t id);
	};
}

#endif