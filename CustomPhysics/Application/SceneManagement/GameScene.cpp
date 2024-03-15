#include "GameScene.h"
#include "../GameObjects/ClothGameObject.h"

namespace CustomApplication
{
	void GameScene::AddGameActorInternal(StaticGameObject* staticGameObject)
	{
		m_staticGameObjects[m_staticGameObjectCount] = staticGameObject;
		m_staticGameObjectCount++;
	}

	void GameScene::AddGameActorInternal(DynamicGameObject* dynamicGameObject)
	{
		m_dynamicGameObjects[m_dynamicGameObjectCount] = dynamicGameObject;
		m_dynamicGameObjectCount++;
	}

	void GameScene::AddGameActorInternal(ClothGameObject* clothGameObject)
	{
		m_clothGameObjects[m_clothGameObjectCount] = clothGameObject;
		m_clothGameObjectCount++;
	}

	void GameScene::AddGameActorInternal(VehicleGameObject* vehicleGameObject)
	{
		m_vehicleGameObjects[m_vehicleGameObjectCount] = vehicleGameObject;
		m_vehicleGameObjectCount++;
	}

	void GameScene::AddGameActor(GameObject* gameObject)
	{
		GameObject::Type underlayingType = gameObject->GetType();

		if (underlayingType == GameObject::Type::Static)
		{
			StaticGameObject* staticGameObject = (StaticGameObject*) gameObject;
			AddGameActorInternal(staticGameObject);
			return;
		}

		if (underlayingType == GameObject::Type::Dynamic)
		{
			DynamicGameObject* dynamicGameObject = (DynamicGameObject*) gameObject;
			AddGameActorInternal(dynamicGameObject);
			return;
		}

		if (underlayingType == GameObject::Type::Cloth)
		{
			ClothGameObject* clothGameObject = (ClothGameObject*) gameObject;
			AddGameActorInternal(clothGameObject);
			return;
		}

		if (underlayingType == GameObject::Type::Vehicle)
		{
			VehicleGameObject* vehicleGameObject = (VehicleGameObject*) gameObject;
			AddGameActorInternal(vehicleGameObject);
			return;
		}

		std::printf("Unknown underlaying GameObject type!\n");
		return;
	}

	void GameScene::Init(const PhysicsEngine::PhysicsEngine* physicsEngine)
	{
		m_staticGameObjectCount = 0;
		m_dynamicGameObjectCount = 0;

		m_staticGameObjects = new GameObject* [k_maxStaticGameObjects];
		m_dynamicGameObjects = new GameObject* [k_maxDynamicGameObjects];
		m_clothGameObjects = new GameObject* [k_maxClothGameObjects];
		m_vehicleGameObjects = new GameObject* [k_maxVehicleGameObjects];

		m_gameObjectFactory = new GameObjectFactory();
		m_gameObjectFactory->Init(const_cast<PhysicsEngine::ActorFactory*>(physicsEngine->GetActorFactory()));
	}

	void GameScene::Update(float dt)
	{
		// TODO: Instead of forcing gameobjects to update, do more data oriented appoarch
		// PhysicsComponets->Update(); CustomComponentA->Update();
		// Maybe it is worth having all custom components in separate container?
	}

	void GameScene::Lock()
	{
		((PhysicsEngine::Scene*) m_physicsScene)->Lock();
	}

	void GameScene::Unlock()
	{
		((PhysicsEngine::Scene*) m_physicsScene)->Unlock();
	}

	const void* GameScene::GetPhysicsScene() const
	{
		return m_physicsScene;
	}

	const GameObject** GameScene::GetStaticGameObjects() const
	{
		return const_cast<const GameObject**>(m_staticGameObjects);
	}

	const uint32_t GameScene::GetStaticGameObjectsCount() const
	{
		return m_staticGameObjectCount;
	}

	const GameObject** GameScene::GetDynamicGameObjects() const
	{
		return const_cast<const GameObject**>(m_dynamicGameObjects);
	}

	const uint32_t GameScene::GetDynamicGameObjectCount() const
	{
		return m_dynamicGameObjectCount;
	}

	const GameObject** GameScene::GetClothGameObjects() const
	{
		return const_cast<const GameObject**>(m_clothGameObjects);
	}

	const uint32_t GameScene::GetClothGameObjectCount() const
	{
		return m_clothGameObjectCount;
	}

	const GameObject** GameScene::GetVehicleGameObjects() const
	{
		return const_cast<const GameObject**>(m_vehicleGameObjects);
	}

	const uint32_t GameScene::GetVehicleGameObjectsCount() const
	{
		return m_vehicleGameObjectCount;
	}

}
