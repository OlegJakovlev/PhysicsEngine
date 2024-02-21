#pragma once

#ifndef CudaContextManager_H
#define CudaContextManager_H

#include "PxPhysicsAPI.h"

namespace PhysicsEngine
{
	class CudaContextManager
	{
	private:
		physx::PxCudaContextManager* m_cudaContextManager;

	public:
		bool Init();
		const physx::PxCudaContextManager* GetContextManager() const;
	};
}

#endif