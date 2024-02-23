#pragma once

#ifndef GameScene_H
#define GameScene_H

#include "../GameObjects/GameObject.h"
#include "../GameObjects/StaticGameObject.h"
#include "../GameObjects/DynamicGameObject.h"
#include "../../PhysicsEngine/Engine/PhysicsEngine.h"
#include "../GameObjects/GameObjectFactory.h"

namespace CustomApplication
{
	class GameScene
	{
	public:
		class ScopedLock
		{
		public:
			ScopedLock(GameScene* gameScene) : m_gameScene(gameScene)
			{
				m_gameScene->Lock();
			}

			~ScopedLock()
			{
				m_gameScene->Unlock();
			}

		private:
			GameScene* m_gameScene;
		};

	private:
		GameObject** m_staticGameObjects;
		GameObject** m_dynamicGameObjects;
		uint32_t m_staticGameObjectCount;
		uint32_t m_dynamicGameObjectCount;

		void AddGameActorInternal(StaticGameObject* staticGameObject);
		void AddGameActorInternal(DynamicGameObject* dynamicGameObject);

	protected:
		const int k_maxStaticGameObjects = 512;
		const int k_maxDynamicGameObjects = 512;

		GameObjectFactory* m_gameObjectFactory;
		void* m_physicsScene;

		void AddGameActor(GameObject* gameObject);

	public:
		virtual void Init(const PhysicsEngine::PhysicsEngine* physicsEngine);
		virtual void Update(float dt);

		void Lock();
		void Unlock();

		const void* GetPhysicsScene() const;
		const GameObject** GetStaticActors() const;
		const uint32_t GetStaticActorCount() const;
		const GameObject** GetDynamicActors() const;
		const uint32_t GetDynamicActorCount() const;
	};
}


#endif