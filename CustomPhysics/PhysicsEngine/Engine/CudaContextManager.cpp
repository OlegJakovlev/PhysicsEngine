#include "CudaContextManager.h"

bool CudaContextManager::Init()
{
	/* Does not work?
	physx::PxTask::CudaContextManagerDesc cudaContextManagerDesc;
	auto mCudaContextManager = physx::PxTask::createCudaContextManager(m_cudaContextManagerDesc);
	m_cudaContextManagerDesc = pxTask::createCudaContextManager(cudaContextManagerDesc);
	*/
	return false;
}

const physx::PxCudaContextManager* CudaContextManager::GetContextManager() const
{
	return m_cudaContextManager;
}
