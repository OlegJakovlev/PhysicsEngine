#pragma once

#ifndef PhysxEvent_H
#define PhysxEvent_H

#include <cstdint>
#include <string>

namespace PhysicsEngine
{
	class PhysxEvent
	{
	protected:
		enum Type
		{
			None, // something went wrong
			AddActor,
		};

		PhysxEvent() = delete;
		PhysxEvent(uint64_t id);
		uint64_t m_eventId;
		uint32_t m_size;
		Type m_type;

	public:
		virtual void Apply() const = 0;
		virtual void Invert() const = 0;
		virtual void Serialize(char* buffer) const = 0;
		virtual std::string SerializeDebug() const = 0;

		uint32_t GetSize() const;
		uint32_t GetType() const;
	};
}

#endif