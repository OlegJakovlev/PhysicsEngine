#include "PhysxEvent.h"

namespace PhysicsEngine
{
	PhysxEvent::PhysxEvent(uint64_t id)
	{
		m_eventId = id;
		m_size = 0;
		m_type = Type::None;
	}

	uint32_t PhysxEvent::GetSize() const
	{
		return m_size;
	}

	uint32_t PhysxEvent::GetType() const
	{
		return m_type;
	}
}
