#pragma once

#ifndef VisualDebugger_H
#define VisualDebugger_H

#include "../GlobalDefine.h"

#ifdef REMOTE_VISUAL_DEBUG
#include "PxPhysicsAPI.h"

namespace PhysicsEngine
{
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
}

#endif
#endif