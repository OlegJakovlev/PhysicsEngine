#pragma once

#ifndef GameScene_H
#define GameScene_H

#include "../GameObjects/GameObject.h"
#include "../GameObjects/StaticGameObject.h"
#include "../GameObjects/DynamicGameObject.h"
#include "../GameObjects/ClothGameObject.h"
#include "../../PhysicsEngine/Engine/PhysicsEngine.h"
#include "../GameObjects/GameObjectFactory.h"
#include "../GameObjects/VehicleGameObject.h"

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
		GameObject** m_clothGameObjects;
		GameObject** m_vehicleGameObjects;

		uint32_t m_staticGameObjectCount;
		uint32_t m_dynamicGameObjectCount;
		uint32_t m_clothGameObjectCount;
		uint32_t m_vehicleGameObjectCount;

		void AddGameActorInternal(StaticGameObject* staticGameObject);
		void AddGameActorInternal(DynamicGameObject* dynamicGameObject);
		void AddGameActorInternal(ClothGameObject* clothGameObject);
		void AddGameActorInternal(VehicleGameObject* vehicleGameObject);

	protected:
		const int k_maxStaticGameObjects = 512;
		const int k_maxDynamicGameObjects = 512;
		const int k_maxClothGameObjects = 128;
		const int k_maxVehicleGameObjects = 4;

		GameObjectFactory* m_gameObjectFactory;
		void* m_physicsScene;

		void AddGameActor(GameObject* gameObject);

	public:
		virtual void Init(const PhysicsEngine::PhysicsEngine* physicsEngine);
		virtual void Update(float dt);

		void Lock();
		void Unlock();

		const void* GetPhysicsScene() const;

		const GameObject** GetStaticGameObjects() const;
		const uint32_t GetStaticGameObjectsCount() const;

		const GameObject** GetDynamicGameObjects() const;
		const uint32_t GetDynamicGameObjectCount() const;

		const GameObject** GetClothGameObjects() const;
		const uint32_t GetClothGameObjectCount() const;

		const GameObject** GetVehicleGameObjects() const;
		const uint32_t GetVehicleGameObjectsCount() const;
	};
}


#endif