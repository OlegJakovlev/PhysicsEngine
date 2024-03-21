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
		float m_lineWidth;
		bool m_disableLighting;
		bool m_ignoreRender;

	public:
		RenderData();
		RenderData(const RenderData&) = delete;
		RenderData& operator=(const RenderData&) = delete;

		void SetColour(const physx::PxVec3* newColor);
		const physx::PxVec3 const* GetColor() const;

		bool GetIsLightingDisabled() const;
		void SetLightingDisabled(bool isDisabled);

		float GetLineWidth() const;
		void SetLineWidth(float width);

		bool GetIgnoreRender() const;
		void SetIgnoreRender(bool ignoreRender);
	};
}

#endif