#pragma once

#ifndef RenderData_H
#define RenderData_H

#include "PxPhysicsAPI.h"

namespace CustomApplication
{
	struct RenderData
	{
		physx::PxVec3* m_color;
		physx::PxClothMeshDesc* m_clothMeshDesc;
	};
}

#endif