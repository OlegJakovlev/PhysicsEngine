#include "GeometryFactory.h"

namespace PhysicsEngine
{
	bool GeometryFactory::Init(const physx::PxPhysics* physics, const physx::PxCooking* cooking)
	{
		m_physics = physics;
		m_cooker = cooking;

		return true;
	}

	physx::PxGeometry* GeometryFactory::Create(const physx::PxGeometryHolder& geoData) const
	{
		physx::PxGeometry* resultGeometry = nullptr;

		switch (geoData.getType())
		{
			case physx::PxGeometryType::eBOX:
				resultGeometry = CreateBox(geoData.box().halfExtents);
				break;

			case physx::PxGeometryType::eCAPSULE:
				resultGeometry = CreateCapsule(physx::PxVec2(geoData.capsule().radius, geoData.capsule().halfHeight));
				break;

			case physx::PxGeometryType::eCONVEXMESH:
				resultGeometry = CreateConvexMesh(geoData.convexMesh().convexMesh->getVertices(),
												  geoData.convexMesh().convexMesh->getNbVertices());
				break;

			case physx::PxGeometryType::eHEIGHTFIELD:
				//physx::PxHeightFieldGeometry& heightfieldData = geom.heightField();
				break;

			case physx::PxGeometryType::ePLANE:
				resultGeometry = CreatePlane();
				break;

			case physx::PxGeometryType::eSPHERE:
				resultGeometry = CreateSphere(geoData.sphere().radius);
				break;

			case physx::PxGeometryType::eTRIANGLEMESH:
				//geometry = geoFactory->CreateTriangleMesh(geom.triangleMesh().getVertices(), geom.triangleMesh().getTriangles());
				break;
		}

		return resultGeometry;
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
		physx::PxConvexMeshDesc meshDesc;
		meshDesc.points.count = (physx::PxU32) verts.size();
		meshDesc.points.stride = sizeof(physx::PxVec3);
		meshDesc.points.data = &verts.front();
		meshDesc.flags = physx::PxConvexFlag::eCOMPUTE_CONVEX;
		meshDesc.vertexLimit = 256;

		return new physx::PxConvexMeshGeometry(CookMesh(meshDesc));
	}

	physx::PxGeometry* GeometryFactory::CreateConvexMesh(const physx::PxVec3* verts, const uint32_t vertAmount) const
	{
		physx::PxConvexMeshDesc meshDesc;
		meshDesc.points.count = vertAmount;
		meshDesc.points.stride = sizeof(physx::PxVec3);
		meshDesc.points.data = &verts;
		meshDesc.flags = physx::PxConvexFlag::eCOMPUTE_CONVEX;
		meshDesc.vertexLimit = 256;

		return new physx::PxConvexMeshGeometry(CookMesh(meshDesc));
	}

	physx::PxGeometry* GeometryFactory::CreateTriangleMesh(const std::vector<physx::PxVec3>& verts, const std::vector<physx::PxU32>& trigs) const
	{
		return new physx::PxTriangleMeshGeometry();
	}

	physx::PxGeometry* GeometryFactory::CreateHeightFieldMesh(const std::vector<physx::PxVec3>& verts, const physx::PxVec2* heightFieldSize, const physx::PxVec3* scaling) const
	{
		physx::PxHeightFieldDesc heightFieldDesc;
		heightFieldDesc.nbColumns = heightFieldSize->x;
		heightFieldDesc.nbRows = heightFieldSize->y;
		heightFieldDesc.convexEdgeThreshold = 0;
		heightFieldDesc.thickness = 0;
		heightFieldDesc.samples.data = &verts;
		heightFieldDesc.samples.stride = sizeof(physx::PxHeightFieldSample);

		return new physx::PxHeightFieldGeometry(CookMesh(heightFieldDesc), physx::PxMeshGeometryFlags(), scaling->y, scaling->x, scaling->z);
	}

	// https://docs.nvidia.com/gameworks/content/gameworkslibrary/physx/guide/Manual/Geometry.html
	physx::PxGeometry* GeometryFactory::CreateHeightFieldMesh(const physx::PxHeightFieldSample* samples, const physx::PxVec2* heightFieldSize, const physx::PxVec3* scaling)
	{
		physx::PxHeightFieldDesc heightFieldDesc;
		heightFieldDesc.nbColumns = heightFieldSize->x;
		heightFieldDesc.nbRows = heightFieldSize->y;
		heightFieldDesc.convexEdgeThreshold = 0;
		heightFieldDesc.thickness = 0;
		heightFieldDesc.samples.data = &samples;
		heightFieldDesc.samples.stride = sizeof(physx::PxHeightFieldSample);

		return new physx::PxHeightFieldGeometry(CookMesh(heightFieldDesc), physx::PxMeshGeometryFlags(), scaling->y, scaling->x, scaling->z);
	}

	physx::PxHeightField* GeometryFactory::CookMesh(const physx::PxHeightFieldDesc& meshDesc) const
	{
		physx::PxDefaultMemoryOutputStream stream;

		if (!m_cooker->cookHeightField(meshDesc, stream))
		{
			std::printf("PxHeightField::CookMesh, cooking failed.");
			return nullptr;
		}

		physx::PxDefaultMemoryInputData input(stream.getData(), stream.getSize());
		return const_cast<physx::PxPhysics*>(m_physics)->createHeightField(input);
	}

	physx::PxConvexMesh* GeometryFactory::CookMesh(const physx::PxConvexMeshDesc& meshDesc) const
	{
		physx::PxDefaultMemoryOutputStream stream;

		if (!m_cooker->cookConvexMesh(meshDesc, stream))
		{
			std::printf("ConvexMesh::CookMesh, cooking failed.");
			return nullptr;
		}

		physx::PxDefaultMemoryInputData input(stream.getData(), stream.getSize());
		return const_cast<physx::PxPhysics*>(m_physics)->createConvexMesh(input);
	}
}
