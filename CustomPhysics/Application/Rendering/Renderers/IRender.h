#pragma once

#ifndef IRender_H
#define IRender_H

#include "PxPhysicsAPI.h"
#include "../RenderData.h"
#include "../Camera.h"
#include "../../GameObjects/GameObject.h"

namespace CustomApplication
{
	class IRender
	{
	protected:
		virtual void DrawPlane() const = 0;
		virtual void DrawSphere(const physx::PxGeometryHolder& geometry) const = 0;
		virtual void DrawBox(const physx::PxGeometryHolder& geometry) const = 0;
		virtual void DrawCapsule(const physx::PxGeometryHolder& geometry) const = 0;
		virtual void DrawConvexMesh(const physx::PxGeometryHolder& geometry) const = 0;
		virtual void DrawTriangleMesh(const physx::PxGeometryHolder& geometry) const = 0;
		virtual void DrawHeightField(const physx::PxGeometryHolder& geometry) const = 0;
		virtual void RenderGeometry(const physx::PxGeometryHolder& geometry) const = 0;
		virtual void RenderCloth(const physx::PxCloth* cloth, const RenderData* renderData) const = 0;

	public:
		virtual bool Init(const char* window_name, int width, int height) = 0;
		virtual void PostInit(const Camera* camera, const physx::PxVec3* defaultColor, const physx::PxVec3* backgroundColor) = 0;

		virtual void Clear() = 0;
		virtual void Render(const GameObject** gameActors, const physx::PxU32 numActors) = 0;
		virtual void RenderDataBuffer(const physx::PxRenderBuffer& data, const physx::PxReal line_width = 1.0f) = 0;
		virtual void FinishRender() = 0;

		virtual void Reshape(int width, int height) = 0;
		virtual void Idle() = 0;
	};
}


#endif