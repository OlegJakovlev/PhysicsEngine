#pragma once

#ifndef TaskManager_H
#define TaskManager_H

#include "PxPhysicsAPI.h"
#include "Dispatcher.h"

// Unfortunately due to the current linking, this class can not be utilized
class TaskManager
{
private:
	physx::PxTaskManager* g_taskManager;

public:
	bool Init(const physx::PxFoundation* foundation, const Dispatcher* dispatcher);
	const physx::PxTaskManager* GetTaskManager() const;
};

#endif