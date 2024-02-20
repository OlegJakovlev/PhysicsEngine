#pragma once

#ifndef ShapeCreator_H
#define ShapeCreator_H

#include "../Engine/Actors/StaticActor.h"
#include "../Engine/Actors/DynamicActor.h"
#include "../Databases/MaterialDatabase.h"

namespace PhysicsEngine
{
	class ShapeCreator
	{
	private:
		const MaterialDatabase* m_materialDatabase;

		physx::PxShape* CreateShapeInternal(StaticActor* actor, const physx::PxGeometry* geometry, const uint32_t materialKey) const;
		physx::PxShape* CreateShapeInternal(DynamicActor* actor, const physx::PxGeometry* geometry, const uint32_t materialKey) const;

		void QueryMaterial(const uint32_t materialKey, const physx::PxMaterial*& material) const;
	public:
		bool Init(const MaterialDatabase* database);

		// TODO: API Exposed
		void CreateShape(void* actor, const physx::PxGeometry* geometry, const uint32_t materialKey) const;
		void CreateTrigger(void* actor, const physx::PxGeometry* geometry, const uint32_t materialKey) const;
	};
}

#endif