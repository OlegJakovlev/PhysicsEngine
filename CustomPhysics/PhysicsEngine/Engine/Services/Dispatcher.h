#pragma once

#ifndef Dispatcher_H
#define Dispatcher_H

#include "PxPhysicsAPI.h"
#include "cudamanager/PxCudaContextManager.h"

namespace PhysicsEngine
{
	class Dispatcher
	{
	private:
		physx::PxCpuDispatcher* m_cpuDispatcher;
		physx::PxGpuDispatcher* m_gpuDispatcher;
		physx::PxCudaContextManager* m_cudaContextManager;

	public:
		bool Init(const physx::PxFoundation* foundation);
		void Release();

		const physx::PxCpuDispatcher* GetCPU() const;
		const physx::PxGpuDispatcher* GetGPU() const;
	};
}

#endif

