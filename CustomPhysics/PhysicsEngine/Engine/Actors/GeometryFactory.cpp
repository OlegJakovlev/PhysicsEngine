#include "GeometryFactory.h"

namespace PhysicsEngine
{
	physx::PxGeometry* GeometryFactory::Create(uint32_t geoType) const
	{
		switch (geoType)
		{
			case physx::PxGeometryType::eBOX:
				break;

			case physx::PxGeometryType::eCAPSULE:
				break;

			case physx::PxGeometryType::eCONVEXMESH:
				break;

			case physx::PxGeometryType::eHEIGHTFIELD:
				break;

			case physx::PxGeometryType::ePLANE:
				break;

			case physx::PxGeometryType::eSPHERE:
				break;

			case physx::PxGeometryType::eTRIANGLEMESH:
				break;
		}

		return nullptr;
	}

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
}
