#pragma once

#ifndef ClothRenderData_H
#define ClothRenderData_H

#include "PxPhysicsAPI.h"
#include "RenderData.h"

namespace CustomApplication
{
	struct ClothRenderData : public RenderData
	{
		physx::PxU32* quads;
	};
}

#endif