#include "Dispatcher.h"
#include <thread>

bool Dispatcher::Init()
{
	uint8_t availableThreads = std::thread::hardware_concurrency();
	g_cpuDispatcher = physx::PxDefaultCpuDispatcherCreate(availableThreads);
	g_gpuDispatcher = nullptr;
	return true;
}

const physx::PxCpuDispatcher* Dispatcher::GetCPU() const
{
	return g_cpuDispatcher;
}

const physx::PxGpuDispatcher* Dispatcher::GetGPU() const
{
	return g_gpuDispatcher;
}

