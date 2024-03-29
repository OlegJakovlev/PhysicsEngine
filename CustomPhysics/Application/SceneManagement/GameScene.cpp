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

	void GameScene::AddGameActorInternal(CustomRenderGameObject* customRenderObject)
	{
		m_customRenderGameObjects[m_customRenderGameObjectCount] = customRenderObject;
		m_customRenderGameObjectCount++;
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

		if (underlayingType == GameObject::Type::Custom)
		{
			CustomRenderGameObject* customRenderObject = (CustomRenderGameObject*) gameObject;
			AddGameActorInternal(customRenderObject);
			return;
		}

		std::printf("Unknown underlaying GameObject type!\n");
		return;
	}

	void GameScene::Init(const PhysicsEngine::PhysicsEngine* physicsEngine)
	{
		m_staticGameObjectCount = 0;
		m_dynamicGameObjectCount = 0;
		m_clothGameObjectCount = 0;
		m_vehicleGameObjectCount = 0;
		m_customRenderGameObjectCount = 0;

		m_staticGameObjects = new GameObject* [k_maxStaticGameObjects];
		m_dynamicGameObjects = new GameObject* [k_maxDynamicGameObjects];
		m_clothGameObjects = new GameObject* [k_maxClothGameObjects];
		m_vehicleGameObjects = new GameObject* [k_maxVehicleGameObjects];
		m_customRenderGameObjects = new GameObject* [k_maxCustomRenderGameObjects];

		m_gameObjectFactory = new GameObjectFactory();
		m_gameObjectFactory->Init(const_cast<PhysicsEngine::ActorFactory*>(physicsEngine->GetActorFactory()));
	}

	void GameScene::ConfigureInternalScene(const PhysicsEngine::PhysicsEngine* physicsEngine)
	{
		// Scene config
		PhysicsEngine::Scene::SceneConfiguration* config = new PhysicsEngine::Scene::SceneConfiguration();
		config->m_gravity = physx::PxVec3(0, -9.81f, 0);
		config->m_enableDemo = true;

		// Collision filter
		config->m_collisionFilter->Init();
		ConfigureCollisionLayers(config->m_collisionFilter);

		// Scene
		PhysicsEngine::SceneManager* sceneManager = const_cast<PhysicsEngine::SceneManager*>(physicsEngine->GetSceneManager());
		PhysicsEngine::Scene* physicsSceneReplica = sceneManager->CreateScene(config);

		m_physicsScene = physicsSceneReplica;
		physicsSceneReplica->LinkEngineScene(this);

		// Add actors to the scene
		const GameObject** allStaticActors = GetStaticGameObjects();
		for (uint32_t i = 0; i < GetStaticGameObjectsCount(); i++)
		{
			physicsSceneReplica->AddActor((PhysicsEngine::Actor*) *allStaticActors[i]->GetPhysicsActorPointer());
		}

		const GameObject** allDynamicActors = GetDynamicGameObjects();
		for (uint32_t i = 0; i < GetDynamicGameObjectCount(); i++)
		{
			physicsSceneReplica->AddActor((PhysicsEngine::Actor*) *allDynamicActors[i]->GetPhysicsActorPointer());
		}

		const GameObject** allClothActors = GetClothGameObjects();
		for (uint32_t i = 0; i < GetClothGameObjectCount(); i++)
		{
			physicsSceneReplica->AddActor((PhysicsEngine::Actor*) *allClothActors[i]->GetPhysicsActorPointer());
		}

		const GameObject** allVehicleActors = GetVehicleGameObjects();
		for (uint32_t i = 0; i < GetVehicleGameObjectsCount(); i++)
		{
			physicsSceneReplica->AddActor((PhysicsEngine::Actor*) *allVehicleActors[i]->GetPhysicsActorPointer());
		}

		// Activate the scene
		sceneManager->AddActiveScene(physicsSceneReplica);
	}

	void GameScene::ConfigureCollisionLayers(PhysicsEngine::CollisionFilter* m_collisionFilter)
	{
		for (int i = 0; i < 32; i++)
		{
			m_collisionFilter->SetCollisionMask((PhysicsEngine::FilterNumericGroup) i, 0xFFFFFFFF);
		}
	}

	void GameScene::Update(float dt)
	{
	}

	// Fixed update based on rendering
	void GameScene::FixedUpdate(float dt)
	{
	}

	// Fixed update based on physics callback
	void GameScene::FixedPhysicsUpdate(float dt)
	{
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

	const GameObject** GameScene::GetCustomRenderObjects() const
	{
		return const_cast<const GameObject**>(m_customRenderGameObjects);
	}

	const uint32_t GameScene::GetCustomRenderObjectCount() const
	{
		return m_customRenderGameObjectCount;
	}

}
