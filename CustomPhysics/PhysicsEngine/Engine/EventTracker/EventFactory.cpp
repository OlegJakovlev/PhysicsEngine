#include "EventFactory.h"
#include "EventTypes/AddActorEvent.h"

uint64_t EventFactory::GenerateId()
{
	return s_lastId++;
}

bool EventFactory::Init()
{
	return true;
}

PhysxEvent* EventFactory::CreateAddActorEvent(const Actor* actor)
{
	return new AddActorEvent(GenerateId(), actor);
}
