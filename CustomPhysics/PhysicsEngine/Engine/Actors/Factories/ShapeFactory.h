#pragma once

#ifndef ShapeCreator_H
#define ShapeCreator_H

#include "../StaticActor.h"
#include "../DynamicActor.h"
#include "../../../Databases/MaterialDatabase.h"

namespace PhysicsEngine
{
	class ShapeFactory
	{
	private:
		const MaterialDatabase* m_materialDatabase;

		physx::PxShape* CreateShapeInternal(StaticActor* actor, const physx::PxGeometry* geometry, const uint32_t materialKey, const physx::PxShape* src = nullptr) const;
		physx::PxShape* CreateShapeInternal(DynamicActor* actor, const physx::PxGeometry* geometry, const uint32_t materialKey, const physx::PxShape* src = nullptr) const;

		void QueryMaterial(const uint32_t materialKey, const physx::PxMaterial*& material) const;
	public:
		bool Init(const MaterialDatabase* database);

		// TODO: API Exposed
#ifdef PHYSICS_DEBUG_MODE
		void CreateShapeDebug(Actor* actor, const physx::PxGeometry* geometry, const uint32_t materialKey) const;
#endif
		void CreateShape(void* actor, const physx::PxGeometry* geometry, const uint32_t materialKey, const physx::PxShape* src = nullptr) const;
		void CreateTrigger(void* actor, const physx::PxGeometry* geometry, const physx::PxShape* src = nullptr) const;
	};
}

#endif