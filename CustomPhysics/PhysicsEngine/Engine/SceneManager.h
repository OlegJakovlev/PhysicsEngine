#pragma once

#ifndef SceneManager_H
#define SceneManager_H

#include <unordered_set>
#include "Scene.h"

namespace PhysicsEngine
{
	class SceneManager
	{
	private:
		std::unordered_set<Scene*> m_activeScenes;

	public:
		bool PostInit(const physx::PxPhysics* physxObject,
					  const physx::PxCpuDispatcher* dispatcherObject);
		void Update(float dt);
		void Release();

		// TODO: API Exposed
		Scene* CreateScene(const Scene::SceneConfiguration* config) const;
		void AddActiveScene(Scene* scene);
		void RemoveActiveScene(Scene* scene);
	};
}

#endif