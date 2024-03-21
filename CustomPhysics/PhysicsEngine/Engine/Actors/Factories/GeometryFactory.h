#pragma once

#ifndef GeometryFactory_H
#define GeometryFactory_H

#include "PxPhysicsAPI.h"
#include <vector>

namespace PhysicsEngine
{
	class GeometryFactory
	{
	private:
		const physx::PxPhysics* m_physics;
		const physx::PxCooking* m_cooker;

		physx::PxConvexMesh* CookMesh(const physx::PxConvexMeshDesc& meshDesc) const;
		physx::PxHeightField* CookMesh(const physx::PxHeightFieldDesc& meshDesc) const;

		physx::PxGeometry* CreateConvexMesh(const physx::PxVec3* verts, const uint32_t vertAmount) const;
		physx::PxGeometry* CreateHeightFieldMesh(const physx::PxHeightFieldSample* samples, const physx::PxVec2* heightFieldSize, const physx::PxVec3* scaling);
	
	public:
		bool Init(const physx::PxPhysics* physics, const physx::PxCooking* cooking);

		// TODO: API Exposed
		physx::PxGeometry* Create(const physx::PxGeometryHolder& geoData) const;

		physx::PxGeometry* CreatePlane() const;

		physx::PxGeometry* CreateSphere(physx::PxReal radius = 1.f) const;

		physx::PxGeometry* CreateBox(physx::PxVec3 dimensions = physx::PxVec3(.5f, .5f, .5f)) const;

		physx::PxGeometry* CreateCapsule(physx::PxVec2 dimensions = physx::PxVec2(1.f, 1.f)) const;

		physx::PxGeometry* CreateConvexMesh(const std::vector<physx::PxVec3>& verts) const;

		physx::PxGeometry* CreateTriangleMesh(const std::vector<physx::PxVec3>& verts, const std::vector<physx::PxU32>& trigs) const;

		physx::PxGeometry* CreateHeightFieldMesh(const std::vector<physx::PxVec3>& verts, const physx::PxVec2* heightFieldSize, const physx::PxVec3* scaling) const;
	};
}

#endif