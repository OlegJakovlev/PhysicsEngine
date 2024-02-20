#pragma once

#ifndef DynamicActor_H
#define DynamicActor_H

#include "Actor.h"

namespace PhysicsEngine
{
	class DynamicActor : public Actor
	{
		friend class ActorFactory;

	private:
		DynamicActor(uint64_t id);
	};
}

#endif