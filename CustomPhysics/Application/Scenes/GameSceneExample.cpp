#include "GameSceneExample.h"
#include "../Utility/Hashing.h"
#include "../GameObjects/StaticGameObject.h"
#include "../GameObjects/DynamicGameObject.h"
#include "../GameObjects/ClothGameObject.h"
#include "../GameObjects/GameObjectFactory.h"
#include "../GlutApp.h"
#include "../Database/ColorDatabase.h"

namespace CustomApplication
{
	void GameSceneExample::Init(const PhysicsEngine::PhysicsEngine* physicsEngine)
	{
		GameScene::Init(physicsEngine);

		// Materials
		PhysicsEngine::MaterialDatabase* materialDatabase = const_cast<PhysicsEngine::MaterialDatabase*>(physicsEngine->GetMaterialDatabase());
		materialDatabase->AddEntry(CRC32_STR("Test"), physx::PxVec3(0, 0, 0.25));
		materialDatabase->AddEntry(CRC32_STR("Bouncy"), physx::PxVec3(1, 1, 1));
		materialDatabase->AddEntry(CRC32_STR("ChassisMaterialDrivable"), physx::PxVec3(0, 0, 0));
		materialDatabase->AddEntry(CRC32_STR("ChassisMaterialNonDrivable"), physx::PxVec3(1, 1, 0));
		materialDatabase->AddEntry(CRC32_STR("Rubber"), physx::PxVec3(1.5, 2.2, 0));

		const PhysicsEngine::GeometryFactory* geoFactory = physicsEngine->GetGeometryFactory();
		const PhysicsEngine::ShapeCreator* shapeCreator = physicsEngine->GetShapeCreator();

		// Colors
		ColorDatabase* colorDatabase = const_cast<ColorDatabase*>(GlutApp::Get()->GetColorDatabase());
		auto asphaltColorEntry = colorDatabase->AddEntry(CRC32_STR("Asphalt"), physx::PxVec3(0.5607, 0.5529, 0.5372));
		auto redColorEntry = colorDatabase->AddEntry(CRC32_STR("Red"), physx::PxVec3(1, 0, 0));
		auto greenColorEntry = colorDatabase->AddEntry(CRC32_STR("Green"), physx::PxVec3(0, 1, 0));
		auto blueColorEntry = colorDatabase->AddEntry(CRC32_STR("Blue"), physx::PxVec3(0, 0, 1));
		auto coalGreyColorEntry = colorDatabase->AddEntry(CRC32_STR("CoalGrey"), physx::PxVec3(0.380, 0.380, 0.380));

		// Geometry
		physx::PxGeometry* planeGeo = geoFactory->CreatePlane();
		physx::PxGeometry* longBox = geoFactory->CreateBox(physx::PxVec3(1, 0.5, 1));
		physx::PxGeometry* box = geoFactory->CreateBox(physx::PxVec3(0.5, 0.5, 0.5));
		physx::PxGeometry* ballGeometry = geoFactory->CreateSphere(0.20f);

		// Game Objects
		GameObject* gamePlane = m_gameObjectFactory->CreateStaticGameObject(physx::PxTransformFromPlaneEquation(physx::PxPlane(physx::PxVec3(0, 1, 0), 0)), GameObject::Layer::Layer_Default);
		shapeCreator->CreateShape(*gamePlane->GetPhysicsActorPointer(), planeGeo, CRC32_STR("Test"));

		GameObject* gameStatic = m_gameObjectFactory->CreateStaticGameObject(physx::PxTransform(physx::PxVec3(0, 0.5f, 0)), GameObject::Layer::Layer_1);
		shapeCreator->CreateShape(*gameStatic->GetPhysicsActorPointer(), longBox, CRC32_STR("Test"));

		GameObject* gameDynamic = m_gameObjectFactory->CreateDynamicGameObject(physx::PxTransform(physx::PxVec3(0, 25, 0)), GameObject::Layer::Layer_2);
		shapeCreator->CreateShape(*gameDynamic->GetPhysicsActorPointer(), box, CRC32_STR("Bouncy"));

		GameObject* gameKinematic = m_gameObjectFactory->CreateKinematicGameObject(physx::PxTransform(physx::PxVec3(5, 0, 0)), GameObject::Layer::Layer_3);
		shapeCreator->CreateShape(*gameKinematic->GetPhysicsActorPointer(), box, CRC32_STR("Test"));

		GameObject* gameTrigger = m_gameObjectFactory->CreateStaticGameObject(physx::PxTransform(physx::PxVec3(0, 10, 0)), GameObject::Layer::Layer_4);
		shapeCreator->CreateTrigger(*gameTrigger->GetPhysicsActorPointer(), box);

		GameObject* clothGameObject = m_gameObjectFactory->CreateClothGameObject(physx::PxTransform(physx::PxVec3(-2.f, 5, 0.f)), physx::PxVec2(4.f, 4.f), physx::PxVec2(40, 40), GameObject::Layer::Layer_1);

		GameObject* ballGameObject = m_gameObjectFactory->CreateDynamicGameObject(physx::PxTransform(physx::PxVec3(0, 0.5f, 0)), GameObject::Layer::Layer_1);
		shapeCreator->CreateShape(*ballGameObject->GetPhysicsActorPointer(), ballGeometry, CRC32_STR("Rubber"));

		// Vehicle configuration
		PhysicsEngine::VehicleData* vehicleData = new PhysicsEngine::VehicleData();

		// Suspension strength and damping rate, wheel mass, tire stiffness and suspension travel direction
		physx::PxVehicleWheelsSimData* wheelsSimData = new physx::PxVehicleWheelsSimData();
		vehicleData->wheelSimData = wheelsSimData;

		// Configuration of the drive model and includes data fields such as engine peak torque,
		// clutch strength, gearing ratios, and Ackermann steering correction
		physx::PxVehicleDriveSimData4W* driveSimData = new physx::PxVehicleDriveSimData4W();
		vehicleData->driveSimData = driveSimData;

		vehicleData->drivableWheels = 4;
		vehicleData->undrivableWheels = 0;
		vehicleData->type = PhysicsEngine::VehicleType::Default4W;

		GameObject* vehicleGameObject = m_gameObjectFactory->CreateVehicleGameObject(physx::PxTransform(physx::PxVec3(0, 10, 0)),
																					 vehicleData,
																					 GameObject::Layer::Layer_1);

		// TODO: Split and refactor into components

		// Create collision / trigger events
		auto OnWakeEnterLambda = []()
		{
			std::printf("Wakey wakey!\n");
		};

		auto OnSleepEnterLambda = []()
		{
			std::printf("Zzzz.....\n");
		};

		auto OnTriggerEnterLambda = [](void* arg1, void* arg2, void* arg3, void* arg4)
		{
			std::printf("On trigger enter!\n");
		};

		auto OnTriggerExitLambda = [](void* arg1, void* arg2, void* arg3, void* arg4)
		{
			std::printf("On trigger exit!\n");
		};

		auto* callbackWakeTest = new PhysicsEngine::VoidCallbackEntry(gameDynamic, OnWakeEnterLambda);
		auto* callbackSleepTest = new PhysicsEngine::VoidCallbackEntry(gameDynamic, OnSleepEnterLambda);

		auto* callbackTriggerEnterTest = new PhysicsEngine::TriggerCallbackEntry(gameTrigger, OnTriggerEnterLambda);
		auto* callbackTriggerExitTest = new PhysicsEngine::TriggerCallbackEntry(gameTrigger, OnTriggerExitLambda);

		auto triggerGameActorRef = (PhysicsEngine::Actor**) gameTrigger->GetPhysicsActorPointer();
		auto gameDynamicActorRef = (PhysicsEngine::Actor**) gameDynamic->GetPhysicsActorPointer();

		(*gameDynamicActorRef)->SubscribeOnWake(*callbackWakeTest);
		(*gameDynamicActorRef)->SubscribeOnSleep(*callbackSleepTest);
		(*triggerGameActorRef)->SubscribeOnTriggerEnter(*callbackTriggerEnterTest);
		(*triggerGameActorRef)->SubscribeOnTriggerExit(*callbackTriggerExitTest);

		// Create render data
		RenderData& planeRenderData = gamePlane->GetRenderData();
		planeRenderData.SetColour(asphaltColorEntry.data);

		RenderData& triggerRenderData = gameTrigger->GetRenderData();
		triggerRenderData.SetColour(redColorEntry.data);

		RenderData& clothRenderData = clothGameObject->GetRenderData();
		clothRenderData.SetColour(greenColorEntry.data);

		RenderData& ballRenderData = ballGameObject->GetRenderData();
		ballRenderData.SetColour(coalGreyColorEntry.data);

		// Add game objects to the scene
		AddGameActor(gamePlane);
		AddGameActor(gameStatic);
		AddGameActor(gameDynamic);
		AddGameActor(gameKinematic);
		AddGameActor(gameTrigger);
		AddGameActor(clothGameObject);
		//AddGameActor(vehicleGameObject);
		AddGameActor(ballGameObject);

		// Clean up
		delete planeGeo;
		delete longBox;
		delete box;
		delete ballGeometry;
	}

	void GameSceneExample::ConfigureCollisionLayers(PhysicsEngine::CollisionFilter* m_collisionFilter)
	{
		// Collision filter
		for (int i = 0; i < 32; i++)
		{
			m_collisionFilter->SetCollisionMask((PhysicsEngine::FilterNumericGroup) i, 0xFFFFFFFF);
		}

		m_collisionFilter->SetCollisionMask(PhysicsEngine::FilterNumericGroup::Index_2,
													PhysicsEngine::FilterGroup::Layer_Default
													| PhysicsEngine::FilterGroup::Layer_1
													| PhysicsEngine::FilterGroup::Layer_2
													| PhysicsEngine::FilterGroup::Layer_4);
	}
}

