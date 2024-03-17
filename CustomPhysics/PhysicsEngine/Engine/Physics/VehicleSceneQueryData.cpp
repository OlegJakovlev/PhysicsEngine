#include "VehicleSceneQueryData.h"

#define SIZEALIGN16(size) (((unsigned)(size)+15)&((unsigned)(~15)));

namespace PhysicsEngine
{
	VehicleSceneQueryData* VehicleSceneQueryData::Create(const physx::PxU32 maxNumWheels)
	{
		const size_t size0 = SIZEALIGN16(sizeof(VehicleSceneQueryData));
		const size_t size1 = SIZEALIGN16(sizeof(physx::PxRaycastQueryResult*) * maxNumWheels);
		const size_t size2 = SIZEALIGN16(sizeof(physx::PxRaycastHit*) * maxNumWheels);
		const size_t totalSize = size0 + size1 + size2;

		VehicleSceneQueryData* result = (VehicleSceneQueryData*) malloc(totalSize);
		if (result == nullptr)
		{
			return nullptr;
		}

		result->m_nbSqResults = maxNumWheels;
		result->m_numQueries = maxNumWheels;

		// Get past the structure
		physx::PxU8* ptr = (physx::PxU8*) result + size0;

		// Set start pointer of m_sqResults array
		result->m_sqResults = (physx::PxRaycastQueryResult*) ptr;
		ptr += size1;

		// Set start pointer of m_sqHitBuffer array
		result->m_sqHitBuffer = (physx::PxRaycastHit*) ptr;
		ptr += size2;

		return result;
	}

	void VehicleSceneQueryData::Release()
	{
		free(this);
	}

	physx::PxBatchQuery* VehicleSceneQueryData::SetUpBatchedSceneQuery(physx::PxScene* physxScene, physx::PxBatchQueryPreFilterShader filter)
	{
		// (maxRaycastsPerExecute, maxSweepsPerExecute, maxOverlapsPerExecute)
		physx::PxBatchQueryDesc sqDesc(m_nbSqResults, 0, 0);

		sqDesc.queryMemory.userRaycastResultBuffer = m_sqResults;
		sqDesc.queryMemory.userRaycastTouchBuffer = m_sqHitBuffer;
		sqDesc.queryMemory.raycastTouchBufferSize = m_nbSqResults;

		sqDesc.preFilterShader = filter;
		physx::PxBatchQuery* batchQuery = physxScene->createBatchQuery(sqDesc);

		return batchQuery;
	}
}