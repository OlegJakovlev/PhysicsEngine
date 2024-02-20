#pragma once

#ifndef VisualDebugger_H
#define VisualDebugger_H

#include "../GlobalDefine.h"

#ifdef DEBUG_MODE
#include "PxPhysicsAPI.h"

class VisualDebugger
{
private:
	const char* host = "localhost";
	const int connectionPort = 5425;
	const int timeoutMillis = 100;
	physx::PxPvd* m_pvd;
	physx::PxPvdTransport* m_transport;

public:
	bool Init(const physx::PxFoundation* foundation);
	void Release();
	const physx::PxPvd* GetPVDService() const;
};

#endif
#endif