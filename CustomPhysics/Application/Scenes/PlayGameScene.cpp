#include "PlayGameScene.h"
#include "../Utility/Hashing.h"
#include "../GlutApp.h"
#include "../Database/ColorDatabase.h"
#include "../Input/InputHandler.h"
#include "../Input/KeyCallbackEntry.h"

#define M_PI 3.14159265358979323846264338327950288

namespace CustomApplication
{
	void PlayGameScene::Init(const PhysicsEngine::PhysicsEngine* physicsEngine)
	{
		GameScene::Init(physicsEngine);

		PhysicsEngine::MaterialDatabase* materialDatabase = const_cast<PhysicsEngine::MaterialDatabase*>(physicsEngine->GetMaterialDatabase());
		const PhysicsEngine::GeometryFactory* geoFactory = physicsEngine->GetGeometryFactory();
		const PhysicsEngine::ShapeCreator* shapeCreator = physicsEngine->GetShapeCreator();

		InputHandler* handler = const_cast<InputHandler*>(GlutApp::Get()->GetInputHandler());
		const Renderer* renderer = GlutApp::Get()->GetRenderer();
		Camera* cameraObject = renderer->GetCamera();

		// Materials
		materialDatabase->AddEntry(CRC32_STR("ChassisMaterialDrivable"), physx::PxVec3(0, 0, 0));
		materialDatabase->AddEntry(CRC32_STR("ChassisMaterialNonDrivable"), physx::PxVec3(1, 1, 0));
		materialDatabase->AddEntry(CRC32_STR("Asphalt"), physx::PxVec3(0, 0, 0)).data;
		materialDatabase->AddEntry(CRC32_STR("Rubber"), physx::PxVec3(1.5, 2.2, 0.828));
		materialDatabase->AddEntry(CRC32_STR("Leather"), physx::PxVec3(0.398, 0.309, 0.752));
		materialDatabase->AddEntry(CRC32_STR("DryGrass"), physx::PxVec3(0.75, 0.38, 0.83));
		materialDatabase->AddEntry(CRC32_STR("WetGrass"), physx::PxVec3(0.6, 0.35, 0.6));
		materialDatabase->AddEntry(CRC32_STR("PVC"), physx::PxVec3(0.45, 0.23, 0.8));

		// Colors
		ColorDatabase* colorDatabase = const_cast<ColorDatabase*>(GlutApp::Get()->GetColorDatabase());
		auto asphaltColor = colorDatabase->AddEntry(CRC32_STR("Asphalt"), physx::PxVec3(0.5607, 0.5529, 0.5372)).data;
		auto grassColor = colorDatabase->AddEntry(CRC32_STR("GrassColor"), physx::PxVec3(0.333, 0.454, 0.1411)).data;
		auto whiteColor = colorDatabase->AddEntry(CRC32_STR("White"), physx::PxVec3(1, 1, 1)).data;
		auto metallicGrey = colorDatabase->AddEntry(CRC32_STR("MetallicGrey"), physx::PxVec3(0.7529, 0.7529, 0.7529)).data;

		// Geometry
		float minHeight = 0.02;
		float minHeightOffset = 0.05;
		float wallHeight = 1;

		float innerWidth = 75;
		float innerDepth = 110;

		float outerWidth = 80;
		float outerDepth = 120;

		float lineWidth = 0.12;
		float lineWidthCentral = 0.24;

		float goalWidth = 7.32;
		float goalHeight = 2.44;

		float cornerPoleHeight = 1.5f;

		physx::PxGeometry* world = geoFactory->CreatePlane();

		physx::PxGeometry* outerWallX = geoFactory->CreateBox(physx::PxVec3(outerWidth / 2, wallHeight / 2, minHeight / 2));
		physx::PxGeometry* outerWallZ = geoFactory->CreateBox(physx::PxVec3(minHeight / 2, wallHeight / 2, outerDepth / 2));

		physx::PxGeometry* innerFieldMarkingX = geoFactory->CreateBox(physx::PxVec3(innerWidth / 2, minHeight / 2, lineWidth / 2));
		physx::PxGeometry* innerFieldMarkingZ = geoFactory->CreateBox(physx::PxVec3(lineWidth / 2, minHeight / 2, innerDepth / 2));

		physx::PxGeometry* goalXWall = geoFactory->CreateCapsule(physx::PxVec2(goalWidth / 2, minHeight / 2));
		physx::PxGeometry* goalYWall = geoFactory->CreateCapsule(physx::PxVec2(minHeight / 2, goalHeight / 2));

		physx::PxGeometry* cornerPole = geoFactory->CreateCapsule(physx::PxVec2(minHeight, cornerPoleHeight / 2));

		// GameObjects
		
		// Gamefield
		auto plane = physx::PxPlane(physx::PxVec3(0, 1, 0), 0);
		GameObject* gamePlane = m_gameObjectFactory->CreateStaticGameObject(physx::PxTransformFromPlaneEquation(plane), GameObject::Layer::Layer_Default);
		shapeCreator->CreateShape(*gamePlane->GetPhysicsActorPointer(), world, CRC32_STR("DryGrass"));
		gamePlane->GetRenderData().SetColour(grassColor);
		AddGameActor(gamePlane);
		
		// Gamefield walls
		GameObject* topOuterWall = m_gameObjectFactory->CreateStaticGameObject(physx::PxTransform(physx::PxVec3(0, wallHeight / 2, outerDepth / 2)), GameObject::Layer::Layer_Default);
		shapeCreator->CreateShape(*topOuterWall->GetPhysicsActorPointer(), outerWallX, CRC32_STR("Asphalt"));
		auto& topOuterWallRenderData = topOuterWall->GetRenderData();
		topOuterWallRenderData.SetColour(metallicGrey);
		topOuterWallRenderData.SetLightingDisabled(true);
		AddGameActor(topOuterWall);

		GameObject* bottomOuterWall = m_gameObjectFactory->CreateStaticGameObject(physx::PxTransform(physx::PxVec3(0, wallHeight / 2, -outerDepth / 2)), GameObject::Layer::Layer_Default);
		shapeCreator->CreateShape(*bottomOuterWall->GetPhysicsActorPointer(), outerWallX, CRC32_STR("Asphalt"));
		auto& bottomOuterWallRenderData = bottomOuterWall->GetRenderData();
		bottomOuterWallRenderData.SetColour(metallicGrey);
		bottomOuterWallRenderData.SetLightingDisabled(true);
		AddGameActor(bottomOuterWall);

		GameObject* leftOuterWall = m_gameObjectFactory->CreateStaticGameObject(physx::PxTransform(physx::PxVec3(-outerWidth / 2, wallHeight / 2, 0)), GameObject::Layer::Layer_Default);
		shapeCreator->CreateShape(*leftOuterWall->GetPhysicsActorPointer(), outerWallZ, CRC32_STR("Asphalt"));
		auto& leftOuterWallRenderData = leftOuterWall->GetRenderData();
		leftOuterWallRenderData.SetColour(metallicGrey);
		leftOuterWallRenderData.SetLightingDisabled(true);
		AddGameActor(leftOuterWall);

		GameObject* rightOuterWall = m_gameObjectFactory->CreateStaticGameObject(physx::PxTransform(physx::PxVec3(outerWidth / 2, wallHeight / 2, 0)), GameObject::Layer::Layer_Default);
		shapeCreator->CreateShape(*rightOuterWall->GetPhysicsActorPointer(), outerWallZ, CRC32_STR("Asphalt"));
		auto& rightOuterWallRenderData = rightOuterWall->GetRenderData();
		rightOuterWallRenderData.SetColour(metallicGrey);
		rightOuterWallRenderData.SetLightingDisabled(true);
		AddGameActor(rightOuterWall);

		// Gamefield marking
		GameObject* topInnerLineMarking = m_gameObjectFactory->CreateStaticGameObject(physx::PxTransform(physx::PxVec3(0, minHeight, innerDepth / 2)), GameObject::Layer::Layer_Default);
		shapeCreator->CreateShape(*topInnerLineMarking->GetPhysicsActorPointer(), innerFieldMarkingX, CRC32_STR("WetGrass"));
		auto& topInnerLineMarkingRenderData = topInnerLineMarking->GetRenderData();
		topInnerLineMarkingRenderData.SetColour(whiteColor);
		topInnerLineMarkingRenderData.SetLightingDisabled(true);
		AddGameActor(topInnerLineMarking);


		GameObject* bottomInnerLineMarking = m_gameObjectFactory->CreateStaticGameObject(physx::PxTransform(physx::PxVec3(0, minHeight, -innerDepth / 2)), GameObject::Layer::Layer_Default);
		shapeCreator->CreateShape(*bottomInnerLineMarking->GetPhysicsActorPointer(), innerFieldMarkingX, CRC32_STR("WetGrass"));
		auto& bottomInnerLineMarkingRenderData = bottomInnerLineMarking->GetRenderData();
		bottomInnerLineMarkingRenderData.SetColour(whiteColor);
		bottomInnerLineMarkingRenderData.SetLightingDisabled(true);
		AddGameActor(bottomInnerLineMarking);


		GameObject* leftInnerLineMarking = m_gameObjectFactory->CreateStaticGameObject(physx::PxTransform(physx::PxVec3(-innerWidth / 2, minHeight, 0)), GameObject::Layer::Layer_Default);
		shapeCreator->CreateShape(*leftInnerLineMarking->GetPhysicsActorPointer(), innerFieldMarkingZ, CRC32_STR("WetGrass"));
		auto& leftInnerLineMarkingRenderData = leftInnerLineMarking->GetRenderData();
		leftInnerLineMarkingRenderData.SetColour(whiteColor);
		leftInnerLineMarkingRenderData.SetLightingDisabled(true);
		AddGameActor(leftInnerLineMarking);

		GameObject* rightInnerLineMarking = m_gameObjectFactory->CreateStaticGameObject(physx::PxTransform(physx::PxVec3(innerWidth / 2, minHeight, 0)), GameObject::Layer::Layer_Default);
		shapeCreator->CreateShape(*rightInnerLineMarking->GetPhysicsActorPointer(), innerFieldMarkingZ, CRC32_STR("WetGrass"));
		auto& rightInnerLineMarkingRenderData = rightInnerLineMarking->GetRenderData();
		rightInnerLineMarkingRenderData.SetColour(whiteColor);
		rightInnerLineMarkingRenderData.SetLightingDisabled(true);
		AddGameActor(rightInnerLineMarking);

		GameObject* centerLineMarking = m_gameObjectFactory->CreateStaticGameObject(physx::PxTransform(physx::PxVec3(0, minHeight, 0)), GameObject::Layer::Layer_Default);
		shapeCreator->CreateShape(*centerLineMarking->GetPhysicsActorPointer(), innerFieldMarkingX, CRC32_STR("WetGrass"));
		auto& centerLineMarkingRenderData = centerLineMarking->GetRenderData();
		centerLineMarkingRenderData.SetColour(whiteColor);
		centerLineMarkingRenderData.SetLightingDisabled(true);
		AddGameActor(centerLineMarking);

		GameObject* centerCircle = m_gameObjectFactory->CreateCustomRenderGameObject();
		auto& centerCircleRenderData = centerCircle->GetRenderData();
		centerCircleRenderData.SetColour(whiteColor);
		centerCircleRenderData.SetLineWidth(lineWidth);

		auto castedCircle = (CustomRenderGameObject*) centerCircle;
		castedCircle->SetCustomRenderCallback(*new AppVoidCallbackEntry(centerCircle, [centerCircle]()
		{
			const Renderer* renderer = GlutApp::Get()->GetRenderer();
			const IRender* impl = renderer->GetRenderingImpl();
			impl->DrawCircle(physx::PxVec3(0, 0.05f, 0), 18.3f, 200, 0, 2 * M_PI, centerCircle->GetRenderData());
		}));
		AddGameActor(centerCircle);

		GameObject* bottomGoalCircle = m_gameObjectFactory->CreateCustomRenderGameObject();
		auto& bottomCircleRenderData = bottomGoalCircle->GetRenderData();
		bottomCircleRenderData.SetColour(whiteColor);
		bottomCircleRenderData.SetLineWidth(lineWidth);

		auto castedBottomCircle = (CustomRenderGameObject*) bottomGoalCircle;
		castedBottomCircle->SetCustomRenderCallback(*new AppVoidCallbackEntry(bottomGoalCircle, [bottomGoalCircle]()
		{
			const Renderer* renderer = GlutApp::Get()->GetRenderer();
			const IRender* impl = renderer->GetRenderingImpl();
			impl->DrawCircle(physx::PxVec3(0, 0.05f, -55), 5.75f, 200, 0, M_PI, bottomGoalCircle->GetRenderData());
		}));
		AddGameActor(bottomGoalCircle);

		GameObject* topGoalCircle = m_gameObjectFactory->CreateCustomRenderGameObject();
		auto& topCircleRenderData = topGoalCircle->GetRenderData();
		topCircleRenderData.SetColour(whiteColor);
		topCircleRenderData.SetLineWidth(lineWidth);

		auto castedTopCircle = (CustomRenderGameObject*) topGoalCircle;
		castedTopCircle->SetCustomRenderCallback(*new AppVoidCallbackEntry(topGoalCircle, [topGoalCircle]()
		{
			const Renderer* renderer = GlutApp::Get()->GetRenderer();
			const IRender* impl = renderer->GetRenderingImpl();
			impl->DrawCircle(physx::PxVec3(0, 0.05f, 55), 5.75f, 200, M_PI, 2 * M_PI, topGoalCircle->GetRenderData());
		}));
		AddGameActor(topGoalCircle);

		// Callbacks and events

		handler->SubscribeOnKeyHold('w', *new KeyCallbackEntry(cameraObject, [](float inputdt)
		{
			auto* appRoot = GlutApp::Get();
			const Renderer* renderer = appRoot->GetRenderer();
			Camera* cameraObject = renderer->GetCamera();
			cameraObject->MoveForward(inputdt);
		}));

		handler->SubscribeOnKeyHold('s', *new KeyCallbackEntry(cameraObject, [](float inputdt)
		{
			auto* appRoot = GlutApp::Get();
			const Renderer* renderer = appRoot->GetRenderer();
			Camera* cameraObject = renderer->GetCamera();
			cameraObject->MoveBackward(inputdt);
		}));

		handler->SubscribeOnKeyHold('a', *new KeyCallbackEntry(cameraObject, [](float inputdt)
		{
			auto* appRoot = GlutApp::Get();
			const Renderer* renderer = appRoot->GetRenderer();
			Camera* cameraObject = renderer->GetCamera();
			cameraObject->MoveLeft(inputdt);
		}));

		handler->SubscribeOnKeyHold('d', *new KeyCallbackEntry(cameraObject, [](float inputdt)
		{
			auto* appRoot = GlutApp::Get();
			const Renderer* renderer = appRoot->GetRenderer();
			Camera* cameraObject = renderer->GetCamera();
			cameraObject->MoveRight(inputdt);
		}));
	}

	void PlayGameScene::FixedPhysicsUpdate(float dt)
	{
		//std::printf("FixedPhysicsUpdate\n");
	}

	void PlayGameScene::FixedUpdate(float dt)
	{
		//std::printf("FixedUpdate\n");
	}

	void PlayGameScene::Update(float dt)
	{
		//std::printf("Update\n");
	}
}
