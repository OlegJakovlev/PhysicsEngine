#pragma once

#ifndef RenderData_H
#define RenderData_H

#include "PxPhysicsAPI.h"

namespace CustomApplication
{
	class RenderData
	{
	private:
		const physx::PxVec3* m_color;

	public:
		RenderData();
		RenderData(const RenderData&) = delete;
		RenderData& operator=(const RenderData&) = delete;
		void SetColour(const physx::PxVec3* newColor);
		
		const physx::PxVec3 const* GetColor() const;
	};
}

#endif