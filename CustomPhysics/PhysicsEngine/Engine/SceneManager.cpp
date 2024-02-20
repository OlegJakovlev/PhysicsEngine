#include "SceneManager.h"
#include "CollisionFilter.h"

namespace PhysicsEngine
{
	bool SceneManager::PostInit(const physx::PxPhysics* physxObject,
								const physx::PxCpuDispatcher* dispatcherObject)
	{
		bool result = true;

		for (Scene* scene : m_activeScenes)
		{
			result |= scene->PostInit(physxObject, dispatcherObject);
		}

		return result;
	}

	void SceneManager::Update(float dt)
	{
		// TODO: in parallel?
		for (Scene* scene : m_activeScenes)
		{
			scene->Update(dt);
		}
	}

	void SceneManager::Release()
	{
		// TODO: in parallel?
		for (Scene* scene : m_activeScenes)
		{
			scene->Release();
		}

		m_activeScenes.clear();
	}

	Scene* SceneManager::CreateScene(const Scene::SceneConfiguration* config) const
	{
		Scene* newScene = new Scene();
		newScene->Init(config);

		return newScene;
	}

	void SceneManager::AddActiveScene(Scene* scene)
	{
		if (m_activeScenes.find(scene) != m_activeScenes.end())
		{
			printf("Trying to add the same scene twice!");
			return;
		}

		m_activeScenes.emplace(scene);
	}

	void SceneManager::RemoveActiveScene(Scene* scene)
	{
		if (m_activeScenes.find(scene) == m_activeScenes.end())
		{
			printf("Can not find scene to remove!");
			return;
		}

		m_activeScenes.erase(scene);
	}
}

