#pragma once

#ifndef EventTracker_H
#define EventTracker_H

#include "../../DemoRecorder/DemoRecorder.h"
#include "EventFactory.h"
#include "../Actors/Actor.h"

namespace PhysicsEngine
{
	// Responsible for recording physx calls in demo history (if enabled)
	class EventTracker
	{
	private:
		DemoRecorder* m_demoRecorder;
		EventFactory* m_eventFactory;

		bool m_enableDemoRecord;

	public:
		bool Init(bool enableDemoRecord);

		// Exposed API calls
		void RegisterAddActorEvent(const Actor* actor) const;
		void RegisterAddForceEvent();
	};
}

#endif