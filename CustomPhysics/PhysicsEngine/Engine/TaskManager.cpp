#include "TaskManager.h"

bool TaskManager::Init(const physx::PxFoundation* foundation, const Dispatcher* dispatcher)
{
	physx::PxFoundation* castedFoundation = const_cast<physx::PxFoundation*>(foundation);
	physx::PxCpuDispatcher* castedCPUDispatcher = const_cast<physx::PxCpuDispatcher*>(dispatcher->GetCPU());
	physx::PxGpuDispatcher* castedGPUDispatcher = const_cast<physx::PxGpuDispatcher*>(dispatcher->GetGPU());

	//g_taskManager = physx::PxTaskManager::createTaskManager(castedFoundation->getErrorCallback(), castedCPUDispatcher, castedGPUDispatcher);
	return g_taskManager != nullptr;
}

const physx::PxTaskManager* TaskManager::GetTaskManager() const
{
	return g_taskManager;
}
