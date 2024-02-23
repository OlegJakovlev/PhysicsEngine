#pragma once

#ifndef Renderer_H
#define Renderer_H

#include "Renderers/IRender.h"
#include "Camera.h"
#include "../UI/HUD.h"
#include "../SceneManagement/GameScene.h"

namespace CustomApplication
{
	class Renderer
	{
	private:
		enum class RenderMode
		{
			DEBUG, NORMAL, BOTH
		};

		IRender* m_impl;
		HUD* m_hud;
		Camera* m_camera;

		RenderMode m_renderMode;

		physx::PxVec3* m_defaultColor;
		physx::PxVec3* m_backgroundColor;

	public:
		bool Init(const char* window_name, int width, int height);
		void PostInit();
		void Release();

		void SetImpl(IRender* impl);

		void Reshape(int width, int height) const;
		void Idle() const;
		void Clear() const;
		void Render(GameScene* gameScene, double dt) const;
		void RenderHUD() const;
		void FinishRender() const;
	};
}


#endif