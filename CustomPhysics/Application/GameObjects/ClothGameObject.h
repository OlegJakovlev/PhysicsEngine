#ifndef ClothGameObject_H
#define ClothGameObject_H

#include "GameObject.h"
#include "../Rendering/ClothRenderData.h"

namespace CustomApplication
{
	class ClothGameObject : public GameObject
	{
		friend class GameObjectFactory;

	private:
		ClothRenderData* m_renderData;

	public:
		ClothGameObject() = delete;
		ClothGameObject(const uint64_t id, physx::PxTransform& transform);

		const ClothRenderData* GetRenderData() const;
	};
}

#endif