#include "Renderer.h"
#include <vector>
#include "../../PhysicsEngine/Engine/Scene.h"
#include "../SceneManagement/GameScene.h"

namespace CustomApplication
{
	bool Renderer::Init(const char* window_name, int width, int height)
	{
		if (!m_impl->Init(window_name, width, height))
		{
			return false;
		}

		m_camera = new Camera(physx::PxVec3(0.0f, 5.0f, 25.0f),
							  physx::PxVec3(0.f, -.1f, -1.f),
							  5.f);

		m_defaultColor = new physx::PxVec3(0.8f, 0.8f, 0.8f);
		m_backgroundColor = new physx::PxVec3(150.f / 255.f, 150.f / 255.f, 150.f / 255.f);

		m_renderMode = RenderMode::BOTH;

		return true;
	}

	void Renderer::PostInit()
	{
		m_impl->PostInit(m_camera, m_defaultColor, m_backgroundColor);
	}

	void Renderer::Release()
	{
	}

	void Renderer::Render(const GameScene* gameScene) const
	{
		if (m_renderMode == RenderMode::DEBUG || m_renderMode == RenderMode::BOTH)
		{
			const PhysicsEngine::Scene* scene = (PhysicsEngine::Scene*) gameScene->GetPhysicsScene();
			m_impl->RenderDataBuffer(scene->GetRenderBuffer());
		}

		if (m_renderMode == RenderMode::NORMAL || m_renderMode == RenderMode::BOTH)
		{
			m_impl->Render(gameScene->GetStaticActors(), gameScene->GetStaticActorCount());
			m_impl->Render(gameScene->GetDynamicActors(), gameScene->GetDynamicActorCount());
		}
	}

	void Renderer::RenderHUD() const
	{
		m_hud->Render();
	}

	void Renderer::FinishRender() const
	{
		m_impl->FinishRender();
	}

	void Renderer::SetImpl(IRender* impl)
	{
		m_impl = impl;
	}

	void Renderer::Reshape(int width, int height) const
	{
		m_impl->Reshape(width, height);
	}

	void Renderer::Idle() const
	{
		m_impl->Idle();
	}

	void Renderer::Clear() const
	{
		m_impl->Clear();
	}
}

