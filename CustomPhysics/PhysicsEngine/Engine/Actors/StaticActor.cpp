#include "StaticActor.h"

namespace PhysicsEngine
{
	StaticActor::StaticActor(uint64_t id) : Actor(id, Actor::Type::Static)
	{
	}
}

