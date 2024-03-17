#include "Dispatcher.h"
#include <thread>
#include "../../GlobalDefine.h"

namespace PhysicsEngine
{
	bool Dispatcher::Init(const physx::PxFoundation* foundation)
	{
		uint8_t availableThreads = std::thread::hardware_concurrency();
		m_cpuDispatcher = physx::PxDefaultCpuDispatcherCreate(availableThreads);
		
#if ENABLE_CUDA
		physx::PxCudaContextManagerDesc cudaContextManagerDesc;
		m_cudaContextManager = PxCreateCudaContextManager(*const_cast<physx::PxFoundation*>(foundation), cudaContextManagerDesc);
		m_gpuDispatcher = m_cudaContextManager->getGpuDispatcher();
		return m_cpuDispatcher != nullptr && m_gpuDispatcher != nullptr;
#else
		m_gpuDispatcher = nullptr;
		return m_cpuDispatcher != nullptr;
#endif
	}

	void Dispatcher::Release()
	{
#if ENABLE_CUDA
		m_gpuDispatcher->getCudaContextManager()->release();
		m_gpuDispatcher = nullptr;
#endif

		delete m_cpuDispatcher;
		m_cpuDispatcher = nullptr;
	}

	const physx::PxCpuDispatcher* Dispatcher::GetCPU() const
	{
		return m_cpuDispatcher;
	}

	const physx::PxGpuDispatcher* Dispatcher::GetGPU() const
	{
		return m_gpuDispatcher;
	}
}

