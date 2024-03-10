#pragma once

#ifndef SceneManager_H
#define SceneManager_H

#include <unordered_set>
#include <unordered_map>
#include "Scene.h"

namespace PhysicsEngine
{
	class SceneManager
	{
	private:
		static uint32_t s_lastSceneId;

		std::unordered_set<Scene*> m_currentScenes;
		std::unordered_set<Scene*> m_renderScenes;

		// Contains (physics, render) scene pairs
		std::unordered_map<Scene*, Scene*> m_sceneBuffer;

		bool CreatePhysicsScene(Scene*& renderScene,
								const physx::PxPhysics* physxObject,
								const Dispatcher* dispatcherObject);

	public:
		bool PostInit(const physx::PxPhysics* physxObject,
					  const Dispatcher* dispatcherObject);
		void Update(float dt);
		void Sync();
		void Release();

		// TODO: API Exposed
		Scene* CreateScene(const Scene::SceneConfiguration* config) const;
		void AddActiveScene(Scene* scene);
	};
}

#endif