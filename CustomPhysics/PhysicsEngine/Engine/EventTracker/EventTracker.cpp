#include "EventTracker.h"

namespace PhysicsEngine
{
	EventTracker::~EventTracker()
	{
		delete m_demoRecorder;
		delete m_eventFactory;
	}

	bool EventTracker::Init(uint32_t sceneId, bool enableDemoRecord)
	{
		m_enableDemoRecord = enableDemoRecord;

		if (!m_enableDemoRecord)
		{
			return true;
		}

		m_demoRecorder = new DemoRecorder();

		if (!m_demoRecorder->Init(sceneId))
		{
			std::printf("DemoRecorder init failed!\n");
			return false;
		}

		m_eventFactory = new EventFactory();
		if (!m_eventFactory->Init())
		{
			return false;
		}

		return true;
	}

	void EventTracker::Release()
	{
		if (m_enableDemoRecord)
		{
			m_demoRecorder->StopRecord();
		}
	}

	void EventTracker::RegisterAddActorEvent(const Actor* actor) const
	{
		if (!m_enableDemoRecord) return;

		m_demoRecorder->Record(m_eventFactory->CreateAddActorEvent(actor));
	}

	void EventTracker::RegisterAddForceEvent()
	{
		if (!m_enableDemoRecord) return;

		//m_demoRecorder->Record(m_eventFactory->CreateAddActorEvent());
	}
}
