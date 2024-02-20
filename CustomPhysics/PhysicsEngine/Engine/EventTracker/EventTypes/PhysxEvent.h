#pragma once

#ifndef PhysxEvent_H
#define PhysxEvent_H

#include <cstdint>

class PhysxEvent
{
protected:
	PhysxEvent(uint64_t id);
	uint64_t m_eventId;

public:
	virtual void Apply() = 0;
	virtual void Invert() = 0;
	virtual void Serialize() = 0;
};

#endif