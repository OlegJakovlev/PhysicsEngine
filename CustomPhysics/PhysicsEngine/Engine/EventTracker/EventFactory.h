#pragma once

#ifndef EventFactory_H
#define EventFactory_H

#include <atomic>
#include "EventTypes/PhysxEvent.h"
#include "../Actors/Actor.h"

namespace PhysicsEngine
{
	class EventFactory
	{
	private:
		uint64_t GenerateId();
		std::atomic<uint64_t> s_lastId;

	public:
		bool Init();
		PhysxEvent* CreateAddActorEvent(const Actor* actor);
	};
}

#endif