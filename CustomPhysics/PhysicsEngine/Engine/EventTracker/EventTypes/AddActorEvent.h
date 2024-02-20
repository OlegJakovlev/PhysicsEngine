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

		};

		AddActorEvent(uint64_t id, const Actor* actor);

	public:
		void Apply() override;
		void Invert() override;
		void Serialize() override;
	};
}

#endif
