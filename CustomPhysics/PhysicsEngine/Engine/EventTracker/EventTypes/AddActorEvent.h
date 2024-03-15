#pragma once

#ifndef AddActorEvent_H
#define AddActorEvent_H

#include "PhysxEvent.h"
#include "../../Actors/Actor.h"

namespace PhysicsEngine
{
	class AddActorEvent : public PhysxEvent
	{
		friend class EventFactory;

	protected:
		struct Data
		{
			Type eventType;
			uint64_t actorId;
			ActorType actorType;
		};

		Data m_data;

		AddActorEvent(uint64_t id, const Actor* actor);

	public:
		void Apply() const override;
		void Invert() const override;
		void Serialize(char* buffer) const override;
		std::string SerializeDebug() const override;
	};
}

#endif
