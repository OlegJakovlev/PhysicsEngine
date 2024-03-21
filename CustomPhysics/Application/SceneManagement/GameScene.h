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
#include "../GameObjects/CustomRenderGameObject.h"

namespace CustomApplication
{
	class GameScene
	{
		friend class GameSceneManager;

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
		GameObject** m_customRenderGameObjects;

		uint32_t m_staticGameObjectCount;
		uint32_t m_dynamicGameObjectCount;
		uint32_t m_clothGameObjectCount;
		uint32_t m_vehicleGameObjectCount;
		uint32_t m_customRenderGameObjectCount;

		void AddGameActorInternal(StaticGameObject* staticGameObject);
		void AddGameActorInternal(DynamicGameObject* dynamicGameObject);
		void AddGameActorInternal(ClothGameObject* clothGameObject);
		void AddGameActorInternal(VehicleGameObject* vehicleGameObject);
		void AddGameActorInternal(CustomRenderGameObject* customRenderObject);

		void ConfigureInternalScene(const PhysicsEngine::PhysicsEngine* physicsEngine);

	protected:
		const int k_maxStaticGameObjects = 32;
		const int k_maxDynamicGameObjects = 32;
		const int k_maxClothGameObjects = 32;
		const int k_maxVehicleGameObjects = 4;
		const int k_maxCustomRenderGameObjects = 32;

		GameObjectFactory* m_gameObjectFactory;
		void* m_physicsScene;

		void AddGameActor(GameObject* gameObject);

	public:
		virtual void Init(const PhysicsEngine::PhysicsEngine* physicsEngine);
		virtual void ConfigureCollisionLayers(PhysicsEngine::CollisionFilter* m_collisionFilter);

		virtual void Update(float dt);
		virtual void FixedUpdate(float dt);
		virtual void FixedPhysicsUpdate(float dt);

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

		const GameObject** GetCustomRenderObjects() const;
		const uint32_t GetCustomRenderObjectCount() const;
	};
}


#endif