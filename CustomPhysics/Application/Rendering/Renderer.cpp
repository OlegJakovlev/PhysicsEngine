#include "Renderer.h"
#include <vector>
#include "../../PhysicsEngine/Engine/SceneManagement/Scene.h"
#include "../SceneManagement/GameScene.h"

namespace CustomApplication
{
	bool Renderer::Init(const char* window_name, int width, int height)
	{
		if (!m_impl->Init(window_name, width, height))
		{
			return false;
		}
		
		/*
		m_camera = new Camera(physx::PxVec3(0.0f, 5.f, 15.f),
							  physx::PxVec3(0.f, 0.f, -1.f),
							  500.f);
							  */
		
		m_camera = new Camera(physx::PxVec3(0, 10.f, 0.f),
							  physx::PxVec3(0, -0.2f, 1.f),
							  12.f);

		m_fov = 80.0f;
		m_backgroundColor = new physx::PxVec3(42 / 255.f, 112 / 255.f, 197 / 255.f);

		m_renderMode = RenderMode::BOTH;

		return true;
	}

	void Renderer::PostInit()
	{
		m_impl->PostInit(m_backgroundColor);
	}

	void Renderer::Release()
	{
	}

	void Renderer::Prepare() const
	{
		m_impl->Prepare(m_fov, m_camera->GetPos(), m_camera->GetDir());
	}

	void Renderer::Render(GameScene* gameScene, double dt) const
	{
		GameScene::ScopedLock lock(gameScene);

		if (m_renderMode == RenderMode::DEBUG || m_renderMode == RenderMode::BOTH)
		{
			const PhysicsEngine::Scene* scene = (PhysicsEngine::Scene*) gameScene->GetPhysicsScene();
			m_impl->RenderDataBuffer(scene->GetRenderBuffer());
		}

		if (m_renderMode == RenderMode::NORMAL || m_renderMode == RenderMode::BOTH)
		{
			m_impl->Render(gameScene->GetStaticGameObjects(), gameScene->GetStaticGameObjectsCount());
			m_impl->Render(gameScene->GetDynamicGameObjects(), gameScene->GetDynamicGameObjectCount());
			m_impl->Render(gameScene->GetClothGameObjects(), gameScene->GetClothGameObjectCount());
			m_impl->Render(gameScene->GetVehicleGameObjects(), gameScene->GetVehicleGameObjectsCount());
			m_impl->Render(gameScene->GetCustomRenderObjects(), gameScene->GetCustomRenderObjectCount());
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

	Camera* Renderer::GetCamera() const
	{
		return m_camera;
	}

	const IRender* Renderer::GetRenderingImpl() const
	{
		return m_impl;
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

