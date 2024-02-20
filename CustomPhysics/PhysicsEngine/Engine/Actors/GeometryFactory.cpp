#include "GeometryFactory.h"

physx::PxGeometry* GeometryFactory::CreatePlane() const
{
	return new physx::PxPlaneGeometry();
}

physx::PxGeometry* GeometryFactory::CreateSphere(physx::PxReal radius) const
{
	return new physx::PxSphereGeometry(radius);
}

physx::PxGeometry* GeometryFactory::CreateBox(physx::PxVec3 dimensions) const
{
	return new physx::PxBoxGeometry(dimensions.x, dimensions.y, dimensions.z);
}

physx::PxGeometry* GeometryFactory::CreateCapsule(physx::PxVec2 dimensions) const
{
	return new physx::PxCapsuleGeometry(dimensions.x, dimensions.y);
}

physx::PxGeometry* GeometryFactory::CreateConvexMesh(const std::vector<physx::PxVec3>& verts) const
{
	return new physx::PxConvexMeshGeometry();
}

physx::PxGeometry* GeometryFactory::CreateTriangleMesh(const std::vector<physx::PxVec3>& verts, const std::vector<physx::PxU32>& trigs) const
{
	return new physx::PxTriangleMeshGeometry();
}