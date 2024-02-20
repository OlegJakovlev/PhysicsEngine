#include "GameSceneExample.h"
#include "../../PhysicsEngine/Utility/Hashing.h"
#include "../GameObjects/StaticGameObject.h"
#include "../GameObjects/DynamicGameObject.h"
#include "../GameObjects/GameObjectFactory.h"

void GameSceneExample::Init(const PhysicsEngine* physicsEngine)
{
	GameScene::Init(physicsEngine);

	// Materials
	MaterialDatabase* materialDatabase = const_cast<MaterialDatabase*>(physicsEngine->GetMaterialDatabase());
	MaterialDatabase::QueryResult materialQueryResult;
	materialQueryResult = materialDatabase->AddEntry(CRC32_STR("Test"), physx::PxVec3(0, 0, 0.25));
	materialQueryResult = materialDatabase->AddEntry(CRC32_STR("Bouncy"), physx::PxVec3(1, 1, 1));

	const GeometryFactory* geoFactory = physicsEngine->GetGeometryFactory();
	const ShapeCreator* shapeCreator = physicsEngine->GetShapeCreator();

	// Game Objects
	GameObject* gamePlane = gameObjectFactory->CreateStaticActor(physx::PxTransformFromPlaneEquation(physx::PxPlane(physx::PxVec3(0, 1, 0), 0)), GameObject::Layer::Layer_Default);
	shapeCreator->CreateShape((void*) gamePlane->GetPhysicsActor(), geoFactory->CreatePlane(), CRC32_STR("Test"));

	GameObject* gameStatic = gameObjectFactory->CreateStaticActor(physx::PxTransform(physx::PxVec3(0, 0.5f, 0)), GameObject::Layer::Layer_1);
	shapeCreator->CreateShape((void*) gameStatic->GetPhysicsActor(), geoFactory->CreateBox(physx::PxVec3(1, 0.5, 1)), CRC32_STR("Test"));

	GameObject* gameDynamic = gameObjectFactory->CreateDynamicActor(physx::PxTransform(physx::PxVec3(0, 25, 0)), GameObject::Layer::Layer_2);
	shapeCreator->CreateShape((void*) gameDynamic->GetPhysicsActor(), geoFactory->CreateBox(physx::PxVec3(0.5, 0.5, 0.5)), CRC32_STR("Bouncy"));

	GameObject* gameKinematic = gameObjectFactory->CreateKinematicActor(physx::PxTransform(physx::PxVec3(5, 0, 0)), GameObject::Layer::Layer_3);
	shapeCreator->CreateShape((void*) gameKinematic->GetPhysicsActor(), geoFactory->CreateBox(physx::PxVec3(0.5, 0.5, 0.5)), CRC32_STR("Test"));

	GameObject* gameTrigger = gameObjectFactory->CreateStaticActor(physx::PxTransform(physx::PxVec3(0, 10, 0)), GameObject::Layer::Layer_4);
	shapeCreator->CreateTrigger((void*) gameTrigger->GetPhysicsActor(), geoFactory->CreateBox(physx::PxVec3(0.5, 0.5, 0.5)), CRC32_STR("Default"));

	AddGameActor(gamePlane);
	AddGameActor(gameStatic);
	AddGameActor(gameDynamic);
	AddGameActor(gameKinematic);
	AddGameActor(gameTrigger);

	// Scene config
	Scene::SceneConfiguration* config = new Scene::SceneConfiguration();
	config->m_gravity = physx::PxVec3(0, -9.81f, 0);
	config->m_enableDemo = true;

	// Collision filter
	config->m_collisionFilter.Init();
	for (int i = 0; i < CollisionFilter::k_maxLayers; i++)
	{
		config->m_collisionFilter.SetCollisionMask(i, 0xFFFFFFFF);
	}

	// Scene
	SceneManager* sceneManager = const_cast<SceneManager*>(physicsEngine->GetSceneManager());
	Scene* physicsSceneReplica = sceneManager->CreateScene(config);

	m_physicsScene = physicsSceneReplica;
	physicsSceneReplica->LinkEngineScene(this);

	// Add actors to the scene
	auto allStaticActors = GetStaticActors();
	for (uint32_t i = 0; i < GetStaticActorCount(); i++)
	{
		physicsSceneReplica->AddActor((StaticActor*) allStaticActors[i]->GetPhysicsActor());
	}

	auto allDynamicActors = GetDynamicActors();
	for (uint32_t i = 0; i < GetDynamicActorCount(); i++)
	{
		physicsSceneReplica->AddActor((DynamicActor*) allDynamicActors[i]->GetPhysicsActor());
	}

	// Activate the scene
	sceneManager->AddActiveScene(physicsSceneReplica);
}
