#pragma once

#include "../GlobalDefine.h"

namespace PhysicsEngine
{
	// Responsible for exposing engine API to end application
	struct IPhysicsEngine
	{
		virtual bool Init() = 0;
		virtual bool PostInit() = 0;
		virtual void Prepare() = 0;
		virtual void Update(float dt) = 0;
		virtual void Sync() = 0;
	};
}
