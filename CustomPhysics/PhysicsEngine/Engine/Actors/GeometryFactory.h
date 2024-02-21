#pragma once

#ifndef GeometryFactory_H
#define GeometryFactory_H

#include "PxPhysicsAPI.h"
#include <vector>

namespace PhysicsEngine
{
	class GeometryFactory
	{
	public:
		// TODO: API Exposed
		physx::PxGeometry* Create(uint32_t geoType) const;

		physx::PxGeometry* CreatePlane() const;

		physx::PxGeometry* CreateSphere(physx::PxReal radius = 1.f) const;

		physx::PxGeometry* CreateBox(physx::PxVec3 dimensions = physx::PxVec3(.5f, .5f, .5f)) const;

		physx::PxGeometry* CreateCapsule(physx::PxVec2 dimensions = physx::PxVec2(1.f, 1.f)) const;

		physx::PxGeometry* CreateConvexMesh(const std::vector<physx::PxVec3>& verts) const;

		physx::PxGeometry* CreateTriangleMesh(const std::vector<physx::PxVec3>& verts, const std::vector<physx::PxU32>& trigs) const;
	};
}

#endif