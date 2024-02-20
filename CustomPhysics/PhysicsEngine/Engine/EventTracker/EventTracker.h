#pragma once

#ifndef EventTracker_H
#define EventTracker_H

#include "../../DemoRecorder/DemoRecorder.h"
#include "EventFactory.h"
#include "../Actors/Actor.h"

// Responsible for forwarding calls to PhysX and record them in Demo (if enabled)
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

#endif