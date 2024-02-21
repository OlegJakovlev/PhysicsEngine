#include "GameSceneExample.h"
#include "../Utility/Hashing.h"
#include "../GameObjects/StaticGameObject.h"
#include "../GameObjects/DynamicGameObject.h"
#include "../GameObjects/GameObjectFactory.h"

namespace CustomApplication
{
	void GameSceneExample::Init(const PhysicsEngine::PhysicsEngine* physicsEngine)
	{
		GameScene::Init(physicsEngine);

		// Materials
		PhysicsEngine::MaterialDatabase* materialDatabase = const_cast<PhysicsEngine::MaterialDatabase*>(physicsEngine->GetMaterialDatabase());
		PhysicsEngine::MaterialDatabase::QueryResult materialQueryResult;
		materialQueryResult = materialDatabase->AddEntry(CRC32_STR("Test"), physx::PxVec3(0, 0, 0.25));
		materialQueryResult = materialDatabase->AddEntry(CRC32_STR("Bouncy"), physx::PxVec3(1, 1, 1));

		const PhysicsEngine::GeometryFactory* geoFactory = physicsEngine->GetGeometryFactory();
		const PhysicsEngine::ShapeCreator* shapeCreator = physicsEngine->GetShapeCreator();

		// Geometry
		physx::PxGeometry* planeGeo = geoFactory->CreatePlane();
		physx::PxGeometry* longBox = geoFactory->CreateBox(physx::PxVec3(1, 0.5, 1));
		physx::PxGeometry* box = geoFactory->CreateBox(physx::PxVec3(0.5, 0.5, 0.5));

		// Game Objects
		GameObject* gamePlane = m_gameObjectFactory->CreateStaticActor(physx::PxTransformFromPlaneEquation(physx::PxPlane(physx::PxVec3(0, 1, 0), 0)), GameObject::Layer::Layer_Default);
		shapeCreator->CreateShape(*gamePlane->GetPhysicsActorPointer(), planeGeo, CRC32_STR("Test"));

		GameObject* gameStatic = m_gameObjectFactory->CreateStaticActor(physx::PxTransform(physx::PxVec3(0, 0.5f, 0)), GameObject::Layer::Layer_1);
		shapeCreator->CreateShape(*gameStatic->GetPhysicsActorPointer(), longBox, CRC32_STR("Test"));

		GameObject* gameDynamic = m_gameObjectFactory->CreateDynamicActor(physx::PxTransform(physx::PxVec3(0, 25, 0)), GameObject::Layer::Layer_2);
		shapeCreator->CreateShape(*gameDynamic->GetPhysicsActorPointer(), box, CRC32_STR("Bouncy"));

		GameObject* gameKinematic = m_gameObjectFactory->CreateKinematicActor(physx::PxTransform(physx::PxVec3(5, 0, 0)), GameObject::Layer::Layer_3);
		shapeCreator->CreateShape(*gameKinematic->GetPhysicsActorPointer(), box, CRC32_STR("Test"));

		GameObject* gameTrigger = m_gameObjectFactory->CreateStaticActor(physx::PxTransform(physx::PxVec3(0, 10, 0)), GameObject::Layer::Layer_4);
		shapeCreator->CreateTrigger(*gameTrigger->GetPhysicsActorPointer(), box, CRC32_STR("Default"));

		AddGameActor(gamePlane);
		AddGameActor(gameStatic);
		AddGameActor(gameDynamic);
		AddGameActor(gameKinematic);
		AddGameActor(gameTrigger);

		delete planeGeo;
		delete longBox;
		delete box;

		// Scene config
		PhysicsEngine::Scene::SceneConfiguration* config = new PhysicsEngine::Scene::SceneConfiguration();
		config->m_gravity = physx::PxVec3(0, -9.81f, 0);
		config->m_enableDemo = true;

		// Collision filter
		config->m_collisionFilter->Init();
		for (int i = 0; i < PhysicsEngine::CollisionFilter::k_maxLayers; i++)
		{
			config->m_collisionFilter->SetCollisionMask(i, 0xFFFFFFFF);
		}

		// Scene
		PhysicsEngine::SceneManager* sceneManager = const_cast<PhysicsEngine::SceneManager*>(physicsEngine->GetSceneManager());
		PhysicsEngine::Scene* physicsSceneReplica = sceneManager->CreateScene(config);

		m_physicsScene = physicsSceneReplica;
		physicsSceneReplica->LinkEngineScene(this);

		// Add actors to the scene
		const GameObject** allStaticActors = GetStaticActors();
		const GameObject* test = allStaticActors[0];

		for (uint32_t i = 0; i < GetStaticActorCount(); i++)
		{
			physicsSceneReplica->AddActor((PhysicsEngine::StaticActor*) *allStaticActors[i]->GetPhysicsActorPointer());
		}

		auto allDynamicActors = GetDynamicActors();
		for (uint32_t i = 0; i < GetDynamicActorCount(); i++)
		{
			physicsSceneReplica->AddActor((PhysicsEngine::DynamicActor*) *allDynamicActors[i]->GetPhysicsActorPointer());
		}

		// Activate the scene
		sceneManager->AddActiveScene(physicsSceneReplica);
	}
}

