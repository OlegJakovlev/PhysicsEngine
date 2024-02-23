#include "VisualDebugger.h"
#include "../GlobalDefine.h"
#include "../Engine/PhysicsEngine.h"

namespace PhysicsEngine
{
	bool VisualDebugger::Init(const physx::PxFoundation* foundation)
	{
#ifdef REMOTE_VISUAL_DEBUG
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
#else
		return true;
#endif
	}

	void VisualDebugger::Release()
	{
#ifdef REMOTE_VISUAL_DEBUG
		if (m_transport)
		{
			if (m_transport->isConnected())
			{
				m_transport->disconnect();
				m_transport->release();
			}
		}
		 
		if (m_pvd)
		{
			if (m_pvd->isConnected())
			{
				m_pvd->disconnect();
				m_pvd->release();
			}
		}

		m_transport = nullptr;
		m_pvd = nullptr;
#endif
	}

	const physx::PxPvd* VisualDebugger::GetPVDService() const
	{
		return m_pvd;
	}
}