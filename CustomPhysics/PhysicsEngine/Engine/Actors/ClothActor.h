#pragma once

#ifndef ClothActor_H
#define ClothActor_H

#include "Actor.h"

namespace PhysicsEngine
{
	class ClothActor : public Actor
	{
		friend class ActorFactory;

	private:
		ClothActor(uint64_t id);
	};
}

#endif