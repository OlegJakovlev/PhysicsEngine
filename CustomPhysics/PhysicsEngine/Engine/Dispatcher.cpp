#include "Dispatcher.h"
#include <thread>

namespace PhysicsEngine
{
	bool Dispatcher::Init()
	{
		uint8_t availableThreads = std::thread::hardware_concurrency();
		g_cpuDispatcher = physx::PxDefaultCpuDispatcherCreate(availableThreads);
		g_gpuDispatcher = nullptr;
		return true;
	}

	bool Dispatcher::Init(const CudaContextManager* contextManager)
	{
		return false;
	}

	const physx::PxCpuDispatcher* Dispatcher::GetCPU() const
	{
		return g_cpuDispatcher;
	}

	const physx::PxGpuDispatcher* Dispatcher::GetGPU() const
	{
		return g_gpuDispatcher;
	}
}

