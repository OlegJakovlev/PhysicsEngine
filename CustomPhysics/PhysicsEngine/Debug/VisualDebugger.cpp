#include "VisualDebugger.h"

#ifdef REMOTE_VISUAL_DEBUG
#include "../Engine/PhysicsEngine.h"

namespace PhysicsEngine
{
	bool VisualDebugger::Init(const physx::PxFoundation* foundation)
	{
		m_pvd = physx::PxCreatePvd(const_cast<physx::PxFoundation&>(*foundation));
		m_transport = physx::PxDefaultPvdSocketTransportCreate(host, connectionPort, timeoutMillis);

		if (!m_pvd || !m_transport)
		{
			std::printf("Visual debugger init failed!\n");
			return false;
		}

		bool connectionSuccesful = m_pvd->connect(*m_transport, physx::PxPvdInstrumentationFlag::eALL);

		if (!connectionSuccesful)
		{
			std::printf("Connection to visual debugger failed!\n");
		}

		return connectionSuccesful;
	}

	void VisualDebugger::Release()
	{
		m_transport->release();
		m_pvd->release();

		m_transport = nullptr;
		m_pvd = nullptr;
	}

	const physx::PxPvd* VisualDebugger::GetPVDService() const
	{
		return m_pvd;
	}
}

#endif