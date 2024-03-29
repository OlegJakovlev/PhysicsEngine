#pragma once

#ifndef GlutRenderer_H
#define GlutRenderer_H

#include "IRender.h"

namespace CustomApplication
{
	// Most of shape rendering code is not mine and was provided as part of the assignment template
	class GlutRenderer : public IRender
	{
	private:
		const physx::PxVec3 k_shadowDir = physx::PxVec3(-0.7071067f, -0.7071067f, -0.7071067f);

		const physx::PxReal k_shadowMat[16] =
		{
			1,0,0,0,
			-k_shadowDir.x / k_shadowDir.y, 0, -k_shadowDir.z / k_shadowDir.y, 0,
			0, 0, 1, 0,
			0, 0, 0, 1,
		};

		const float k_PlaneData[36] =
		{
			-1.f, 0.f, -1.f, 0.f, 1.f, 0.f, -1.f, 0.f, 1.f,  0.f, 1.f, 0.f,
			1.f,  0.f, 1.f,  0.f, 1.f, 0.f, -1.f, 0.f, -1.f, 0.f, 1.f, 0.f,
			1.f,  0.f, 1.f,  0.f, 1.f, 0.f, 1.f,  0.f, -1.f, 0.f, 1.f, 0.f,
		};

		const physx::PxVec3 k_shadowColor = physx::PxVec3(0.3f, 0.3f, 0.3f);

		// Is being used for cloth and plane rendering
		// Based on assumption that world plane y = 0;
		float k_specialMinRenderDistance = 1.f;
		const physx::PxVec3* m_backgroundColour;

		int m_renderDetail;
		float m_maxShadowDistance;
		bool m_showShadows;

		void SetupWindow(const char* window_name, int width, int height);
		void SetupRenderStates();
		void SetupLighting();
		void RenderBuffer(float* pVertList, float* pColorList, int type, int num) const;
		void UpdateCameraRender(const float fov, const physx::PxVec3& cameraEye, const physx::PxVec3& cameraDir) const;
		void RenderShapes(PhysicsEngine::Actor* actor, const RenderData& renderData) const;

	public:
		void DrawCircle(const physx::PxVec3& centerOffset, const float radius, const int segments, const float startAngle, const float endAngle, const RenderData& renderData) const override;
		void DrawCircleLine(const physx::PxVec3& centerOffset, const float radius, const int segments, const float startAngle, const float endAngle, const RenderData& renderData) const override;
		void DrawBox(const physx::PxTransform& pose, const physx::PxVec3& halfExtents, const RenderData& renderData) const override;

	protected:
		void DrawPlane() const override;
		void DrawSphere(const physx::PxGeometryHolder& geometry) const override;
		void DrawBox(const physx::PxGeometryHolder& geometry) const override;
		void DrawCapsule(const physx::PxGeometryHolder& geometry) const override;
		void DrawConvexMesh(const physx::PxGeometryHolder& geometry) const override;
		void DrawTriangleMesh(const physx::PxGeometryHolder& geometry) const override;
		void DrawHeightField(const physx::PxGeometryHolder& geometry) const override;
		void RenderGeometry(const physx::PxGeometryHolder& geometry) const override;
		void RenderCloth(const PhysicsEngine::ClothActor* cloth, const RenderData& renderData) const override;

	public:
		bool Init(const char* window_name, int width, int height) override;
		void PostInit(const physx::PxVec3* backgroundColor) override;

		void Clear() override;
		void Prepare(const float fov, physx::PxVec3& pos, physx::PxVec3& dir) override;
		void Render(const GameObject** gameActors, const physx::PxU32 numActors) override;
		void RenderDataBuffer(const physx::PxRenderBuffer& data, const physx::PxReal line_width = 1.0f) override;
		void FinishRender() override;

		void Reshape(int width, int height) override;
		void Idle() override;
	};
}

#endif