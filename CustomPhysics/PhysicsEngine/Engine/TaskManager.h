#pragma once

#ifndef TaskManager_H
#define TaskManager_H

#include "PxPhysicsAPI.h"
#include "Dispatcher.h"

namespace PhysicsEngine
{
	// Unfortunately due to the current Physx version (?) this class can not be utilized
	class TaskManager
	{
	private:
		physx::PxTaskManager* g_taskManager;

	public:
		bool Init(const physx::PxFoundation* foundation, const Dispatcher* dispatcher);
		const physx::PxTaskManager* GetTaskManager() const;
	};
}

#endif