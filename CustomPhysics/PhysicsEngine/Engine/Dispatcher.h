#pragma once

#ifndef Dispatcher_H
#define Dispatcher_H

#include "PxPhysicsAPI.h"
#include "CudaContextManager.h"

class Dispatcher
{
private:
	physx::PxCpuDispatcher* g_cpuDispatcher;

#if PX_WINDOWS
	physx::PxGpuDispatcher* g_gpuDispatcher;
#endif

public:
	bool Init();
	//bool Init(const CudaContextManager* contextManager);

	const physx::PxCpuDispatcher* GetCPU() const;
	const physx::PxGpuDispatcher* GetGPU() const;
};

#endif

