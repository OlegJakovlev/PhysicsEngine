#pragma once

#ifndef BatchQueryWrapper_H
#define BatchQueryWrapper_H

#include <PxPhysicsAPI.h>

namespace PhysicsEngine
{
	class BatchQueryWrapper
	{
	private:
		physx::PxBatchQuery* m_batchQuery;

	public:
		bool Init(int wheelAmount);
		physx::PxBatchQuery const* GetBatchQuery();
	};
}

#endif
