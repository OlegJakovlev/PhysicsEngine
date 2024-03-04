#include "SceneManager.h"
#include "../Physics/CollisionFilter.h"

namespace PhysicsEngine
{
	uint32_t SceneManager::s_lastSceneId = 0;

	bool SceneManager::PostInit(const physx::PxPhysics* physxObject,
								const physx::PxCpuDispatcher* dispatcherObject)
	{
		bool result = true;

		for (Scene* renderScene : m_renderScenes)
		{
			if (!CreatePhysicsScene(renderScene, physxObject, dispatcherObject))
			{
				return false;
			}
		}

		return result;
	}

	bool SceneManager::CreatePhysicsScene(Scene*& renderScene, const physx::PxPhysics* physxObject, const physx::PxCpuDispatcher* dispatcherObject)
	{
		const Scene::SceneConfiguration* realSceneConfiguration = renderScene->m_configuration;

		Scene::SceneConfiguration* renderConfiguration = new Scene::SceneConfiguration(*renderScene->m_configuration);
		renderConfiguration->m_collisionFilter = realSceneConfiguration->m_collisionFilter;
		renderConfiguration->m_gravity = realSceneConfiguration->m_gravity;
		renderConfiguration->m_enableDemo = false;
		renderScene->Init(renderConfiguration);
		renderScene->PostInit(physxObject, dispatcherObject);

		Scene* realScene = new Scene(s_lastSceneId++);

		if (!realScene->Init(realSceneConfiguration))
		{
			return false;
		}

		if (!realScene->PostInit(physxObject, dispatcherObject))
		{
			return false;
		}

		realScene->Sync(renderScene, Scene::SyncState::ALL);

		m_currentScenes.emplace(realScene);
		m_sceneBuffer.emplace(realScene, renderScene);

		return true;
	}

	void SceneManager::Update(float dt)
	{
		for (Scene* scene : m_currentScenes)
		{
			scene->Update(dt);
		}
	}

	void SceneManager::Sync()
	{
		for (Scene* physicsScene : m_currentScenes)
		{
			m_sceneBuffer[physicsScene]->Sync(physicsScene, Scene::SyncState::RUNTIME_UPDATE);
		}
	}

	void SceneManager::Release()
	{
		for (Scene* scene : m_currentScenes)
		{
			scene->Release();
		}

		for (Scene* scene : m_renderScenes)
		{
			scene->Release();
		}

		m_renderScenes.clear();
		m_currentScenes.clear();
	}

	Scene* SceneManager::CreateScene(const Scene::SceneConfiguration* config) const
	{
		Scene* newScene = new Scene(s_lastSceneId++);

		// We don't Init, so we save resources, as we may never add to the active scene pool
		newScene->m_configuration = config;

		return newScene;
	}

	void SceneManager::AddActiveScene(Scene* scene)
	{
		if (m_renderScenes.find(scene) != m_renderScenes.end())
		{
			printf("Trying to add the same scene twice!\n");
			return;
		}

		m_renderScenes.emplace(scene);
	}
}

