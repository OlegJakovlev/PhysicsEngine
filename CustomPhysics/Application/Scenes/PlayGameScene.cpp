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

		PhysicsEngine::ActorFactory* actorFactory = const_cast<PhysicsEngine::ActorFactory*>(physicsEngine->GetActorFactory());
		const PhysicsEngine::GeometryFactory* geoFactory = physicsEngine->GetGeometryFactory();
		const PhysicsEngine::ShapeFactory* shapeCreator = physicsEngine->GetShapeFactory();

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
		materialDatabase->AddEntry(CRC32_STR("Aluminum"), physx::PxVec3(1.2, 1.4, 0.08));

		// Colors
		ColorDatabase* colorDatabase = const_cast<ColorDatabase*>(GlutApp::Get()->GetColorDatabase());
		colorDatabase->AddEntry(CRC32_STR("Asphalt"), physx::PxVec3(0.5607, 0.5529, 0.5372)).data;
		colorDatabase->AddEntry(CRC32_STR("GrassColor"), physx::PxVec3(0.333, 0.454, 0.1411)).data;
		colorDatabase->AddEntry(CRC32_STR("White"), physx::PxVec3(1, 1, 1)).data;
		colorDatabase->AddEntry(CRC32_STR("Reddish"), physx::PxVec3(0.6470, 0.1647, 0.1647)).data;
		colorDatabase->AddEntry(CRC32_STR("OuterWall"), physx::PxVec3(0.23, 0.12, 0.629)).data;

		// Geometry
		float minHeightOffset = 0.05;

		float innerFieldWidth = 75;
		float innerFieldDepth = 110;

		physx::PxGeometry* world = geoFactory->CreatePlane();
		physx::PxGeometry* ballGeometry = geoFactory->CreateSphere(0.20f);

		// Gamefield
		auto plane = physx::PxPlane(physx::PxVec3(0, 1, 0), 0);
		GameObject* gamePlane = m_gameObjectFactory->CreateStaticGameObject(physx::PxTransformFromPlaneEquation(plane), GameObject::Layer::Layer_Default);
		shapeCreator->CreateShape(*gamePlane->GetPhysicsActorPointer(), world, CRC32_STR("DryGrass"));
		gamePlane->GetRenderData().SetColour(colorDatabase->GetEntry(CRC32_STR("GrassColor")).data);
		AddGameActor(gamePlane);

		// Ball
		GameObject* ballGameObject = m_gameObjectFactory->CreateDynamicGameObject(physx::PxTransform(physx::PxVec3(0, 0.5f, 0)), GameObject::Layer::Layer_Default);
		shapeCreator->CreateShape(*ballGameObject->GetPhysicsActorPointer(), ballGeometry, CRC32_STR("Leather"));
		RenderData& ballRenderData = ballGameObject->GetRenderData();
		ballRenderData.SetColour(colorDatabase->GetEntry(CRC32_STR("White")).data);
		AddGameActor(ballGameObject);

		GoalObjects(geoFactory, shapeCreator, innerFieldWidth, innerFieldDepth);
		GameFieldOuterWalls(shapeCreator, geoFactory, minHeightOffset, innerFieldWidth, innerFieldDepth);
		GameFieldMarking(shapeCreator, geoFactory, minHeightOffset, innerFieldWidth, innerFieldDepth);
		CornerPolesAndFlags(geoFactory, shapeCreator, innerFieldWidth, innerFieldDepth);

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

	void PlayGameScene::CornerPolesAndFlags(const PhysicsEngine::GeometryFactory* geoFactory, const PhysicsEngine::ShapeFactory* shapeCreator, float innerFieldWidth, float innerFieldDepth)
	{
		float cornerPoleHeight = 1.5f;
		float cornerPoleDiameter = 0.25f;
		auto poleGeo = geoFactory->CreateCapsule(physx::PxVec2(cornerPoleDiameter / 2, cornerPoleHeight / 2));

		auto colorDatabase = GlutApp::Get()->GetColorDatabase();
		const physx::PxVec3* whiteColor = colorDatabase->GetEntry(CRC32_STR("White")).data;
		const physx::PxVec3* reddishColor = colorDatabase->GetEntry(CRC32_STR("Reddish")).data;

		// Top left
		GameObject* topLeftPole = m_gameObjectFactory->CreateDynamicGameObject(physx::PxTransform(physx::PxVec3(-innerFieldWidth / 2, cornerPoleHeight / 2, innerFieldDepth / 2), physx::PxQuat(physx::PxPi / 2, physx::PxVec3(0, 0, 1))), GameObject::Layer::Layer_Default);
		shapeCreator->CreateShape(*topLeftPole->GetPhysicsActorPointer(), poleGeo, CRC32_STR("PVC"));
		topLeftPole->GetRenderData().SetColour(reddishColor);
		AddGameActor(topLeftPole);

		GameObject* topLeftPoleFlag = m_gameObjectFactory->CreateClothGameObject(physx::PxTransform(physx::PxVec3(-innerFieldWidth / 2, cornerPoleHeight, innerFieldDepth / 2)), physx::PxVec2(0.6, 0.6), physx::PxVec2(10, 10), GameObject::Layer::Layer_Default);
		topLeftPoleFlag->GetRenderData().SetColour(whiteColor);
		AddGameActor(topLeftPoleFlag);

		// Top right
		GameObject* topRightPole = m_gameObjectFactory->CreateDynamicGameObject(physx::PxTransform(physx::PxVec3(innerFieldWidth / 2, cornerPoleHeight / 2, innerFieldDepth / 2), physx::PxQuat(physx::PxPi / 2, physx::PxVec3(0, 0, 1))), GameObject::Layer::Layer_Default);
		shapeCreator->CreateShape(*topRightPole->GetPhysicsActorPointer(), poleGeo, CRC32_STR("PVC"));
		topRightPole->GetRenderData().SetColour(reddishColor);
		AddGameActor(topRightPole);

		GameObject* topRightPoleFlag = m_gameObjectFactory->CreateClothGameObject(physx::PxTransform(physx::PxVec3(innerFieldWidth / 2, cornerPoleHeight, innerFieldDepth / 2)), physx::PxVec2(0.6, 0.6), physx::PxVec2(10, 10), GameObject::Layer::Layer_Default);
		topRightPoleFlag->GetRenderData().SetColour(whiteColor);
		AddGameActor(topRightPoleFlag);

		// Bottom left
		GameObject* bottomLeftPole = m_gameObjectFactory->CreateDynamicGameObject(physx::PxTransform(physx::PxVec3(-innerFieldWidth / 2, cornerPoleHeight / 2, -innerFieldDepth / 2), physx::PxQuat(physx::PxPi / 2, physx::PxVec3(0, 0, 1))), GameObject::Layer::Layer_Default);
		shapeCreator->CreateShape(*bottomLeftPole->GetPhysicsActorPointer(), poleGeo, CRC32_STR("PVC"));
		bottomLeftPole->GetRenderData().SetColour(reddishColor);
		AddGameActor(bottomLeftPole);

		GameObject* bottomLeftPoleFlag = m_gameObjectFactory->CreateClothGameObject(physx::PxTransform(physx::PxVec3(-innerFieldWidth / 2, cornerPoleHeight, -innerFieldDepth / 2)), physx::PxVec2(0.6, 0.6), physx::PxVec2(10, 10), GameObject::Layer::Layer_Default);
		bottomLeftPoleFlag->GetRenderData().SetColour(whiteColor);
		AddGameActor(bottomLeftPoleFlag);

		// Bottom right
		GameObject* bottomRightPole = m_gameObjectFactory->CreateDynamicGameObject(physx::PxTransform(physx::PxVec3(innerFieldWidth / 2, cornerPoleHeight / 2, -innerFieldDepth / 2), physx::PxQuat(physx::PxPi / 2, physx::PxVec3(0, 0, 1))), GameObject::Layer::Layer_Default);
		shapeCreator->CreateShape(*bottomRightPole->GetPhysicsActorPointer(), poleGeo, CRC32_STR("PVC"));
		bottomRightPole->GetRenderData().SetColour(reddishColor);
		AddGameActor(bottomRightPole);

		GameObject* bottomRightPoleFlag = m_gameObjectFactory->CreateClothGameObject(physx::PxTransform(physx::PxVec3(innerFieldWidth / 2, cornerPoleHeight, -innerFieldDepth / 2)), physx::PxVec2(0.6, 0.6), physx::PxVec2(10, 10), GameObject::Layer::Layer_Default);
		bottomRightPoleFlag->GetRenderData().SetColour(whiteColor);
		AddGameActor(bottomRightPoleFlag);
	}

	void PlayGameScene::GoalObjects(const PhysicsEngine::GeometryFactory* geoFactory, const PhysicsEngine::ShapeFactory* shapeCreator, float innerFieldWidth, float innerFieldDepth)
	{
		float goalRadius = 0.06;
		float goalWidth = 7.32;
		float goalHeight = 2.44;
		float goalDepth = 1.524;

		physx::PxGeometry* goalXWall = geoFactory->CreateCapsule(physx::PxVec2(goalRadius, goalWidth / 2));
		physx::PxGeometry* goalYWall = geoFactory->CreateCapsule(physx::PxVec2(goalRadius, goalHeight / 2));
		physx::PxGeometry* goalZWall = geoFactory->CreateCapsule(physx::PxVec2(goalRadius, goalDepth / 2));
		physx::PxGeometry* goalTrigger = geoFactory->CreateBox(physx::PxVec3(goalWidth / 2, goalHeight / 2, goalDepth / 2));

		// Top Goal
		GameObject* topGoalPart = m_gameObjectFactory->CreateDynamicGameObject(physx::PxTransform(physx::PxVec3(0, goalHeight + goalRadius, innerFieldDepth / 2)), GameObject::Layer::Layer_Default);
		shapeCreator->CreateShape(*topGoalPart->GetPhysicsActorPointer(), goalXWall, CRC32_STR("Aluminum"));
		AddGameActor(topGoalPart);

		GameObject* leftGoalPart = m_gameObjectFactory->CreateDynamicGameObject(physx::PxTransform(physx::PxVec3(-goalWidth / 2, goalHeight / 2 + goalRadius, innerFieldDepth / 2), physx::PxQuat(physx::PxPi / 2, physx::PxVec3(0.0f, 0.0f, 1.0f))), GameObject::Layer::Layer_Default);
		shapeCreator->CreateShape(*leftGoalPart->GetPhysicsActorPointer(), goalYWall, CRC32_STR("Aluminum"));
		AddGameActor(leftGoalPart);

		GameObject* rightGoalPart = m_gameObjectFactory->CreateDynamicGameObject(physx::PxTransform(physx::PxVec3(goalWidth / 2, goalHeight / 2 + goalRadius, innerFieldDepth / 2), physx::PxQuat(physx::PxPi / 2, physx::PxVec3(0.0f, 0.0f, 1.0f))), GameObject::Layer::Layer_Default);
		shapeCreator->CreateShape(*rightGoalPart->GetPhysicsActorPointer(), goalYWall, CRC32_STR("Aluminum"));
		AddGameActor(rightGoalPart);

		GameObject* leftBottomGoalPart = m_gameObjectFactory->CreateDynamicGameObject(physx::PxTransform(physx::PxVec3(-goalWidth / 2, goalRadius, innerFieldDepth / 2), physx::PxQuat(physx::PxPi / 2, physx::PxVec3(0.0f, 1.0f, 0.0f))), GameObject::Layer::Layer_Default);
		shapeCreator->CreateShape(*leftBottomGoalPart->GetPhysicsActorPointer(), goalZWall, CRC32_STR("Aluminum"));
		AddGameActor(leftBottomGoalPart);

		GameObject* rightBottomGoalPart = m_gameObjectFactory->CreateDynamicGameObject(physx::PxTransform(physx::PxVec3(goalWidth / 2, goalRadius, innerFieldDepth / 2), physx::PxQuat(physx::PxPi / 2, physx::PxVec3(0.0f, 1.0f, 0.0f))), GameObject::Layer::Layer_Default);
		shapeCreator->CreateShape(*rightBottomGoalPart->GetPhysicsActorPointer(), goalZWall, CRC32_STR("Aluminum"));
		AddGameActor(rightBottomGoalPart);

		GameObject* topGoalTrigger = m_gameObjectFactory->CreateStaticGameObject(physx::PxTransform(physx::PxVec3(0, goalHeight / 2, innerFieldDepth / 2 + goalDepth / 2)), GameObject::Layer::Layer_Default);
		shapeCreator->CreateTrigger(*topGoalTrigger->GetPhysicsActorPointer(), goalTrigger);
		RenderData& topGoalTriggerRenderData = topGoalTrigger->GetRenderData();
		topGoalTriggerRenderData.SetIgnoreRender(true);
		AddGameActor(topGoalTrigger);



		// Bottom Goal
		GameObject* bottomTopGoalPart = m_gameObjectFactory->CreateDynamicGameObject(physx::PxTransform(physx::PxVec3(0, goalHeight + goalRadius, -innerFieldDepth / 2)), GameObject::Layer::Layer_Default);
		shapeCreator->CreateShape(*bottomTopGoalPart->GetPhysicsActorPointer(), goalXWall, CRC32_STR("Aluminum"));
		AddGameActor(bottomTopGoalPart);

		GameObject* bottomLeftGoalPart = m_gameObjectFactory->CreateDynamicGameObject(physx::PxTransform(physx::PxVec3(-goalWidth / 2, goalHeight / 2 + goalRadius, -innerFieldDepth / 2), physx::PxQuat(physx::PxPi / 2, physx::PxVec3(0.0f, 0.0f, 1.0f))), GameObject::Layer::Layer_Default);
		shapeCreator->CreateShape(*bottomLeftGoalPart->GetPhysicsActorPointer(), goalYWall, CRC32_STR("Aluminum"));
		AddGameActor(bottomLeftGoalPart);

		GameObject* bottomRightGoalPart = m_gameObjectFactory->CreateDynamicGameObject(physx::PxTransform(physx::PxVec3(goalWidth / 2, goalHeight / 2 + goalRadius, -innerFieldDepth / 2), physx::PxQuat(physx::PxPi / 2, physx::PxVec3(0.0f, 0.0f, 1.0f))), GameObject::Layer::Layer_Default);
		shapeCreator->CreateShape(*bottomRightGoalPart->GetPhysicsActorPointer(), goalYWall, CRC32_STR("Aluminum"));
		AddGameActor(bottomRightGoalPart);

		GameObject* bottomLeftBottomGoalPart = m_gameObjectFactory->CreateDynamicGameObject(physx::PxTransform(physx::PxVec3(-goalWidth / 2, goalRadius, -innerFieldDepth / 2), physx::PxQuat(physx::PxPi / 2, physx::PxVec3(0.0f, 1.0f, 0.0f))), GameObject::Layer::Layer_Default);
		shapeCreator->CreateShape(*bottomLeftBottomGoalPart->GetPhysicsActorPointer(), goalZWall, CRC32_STR("Aluminum"));
		AddGameActor(bottomLeftBottomGoalPart);

		GameObject* bottomRightBottomGoalPart = m_gameObjectFactory->CreateDynamicGameObject(physx::PxTransform(physx::PxVec3(goalWidth / 2, goalRadius, -innerFieldDepth / 2), physx::PxQuat(physx::PxPi / 2, physx::PxVec3(0.0f, 1.0f, 0.0f))), GameObject::Layer::Layer_Default);
		shapeCreator->CreateShape(*bottomRightBottomGoalPart->GetPhysicsActorPointer(), goalZWall, CRC32_STR("Aluminum"));
		AddGameActor(bottomRightBottomGoalPart);

		GameObject* bottomGoalTrigger = m_gameObjectFactory->CreateStaticGameObject(physx::PxTransform(physx::PxVec3(0, goalHeight / 2, -innerFieldDepth / 2 - goalDepth / 2)), GameObject::Layer::Layer_Default);
		shapeCreator->CreateTrigger(*bottomGoalTrigger->GetPhysicsActorPointer(), goalTrigger);
		RenderData& bottomGoalTriggerRenderData = bottomGoalTrigger->GetRenderData();
		bottomGoalTriggerRenderData.SetIgnoreRender(true);
		AddGameActor(bottomGoalTrigger);
	}

	void PlayGameScene::GameFieldOuterWalls(const PhysicsEngine::ShapeFactory* shapeCreator, const PhysicsEngine::GeometryFactory* geoFactory, float minHeightOffset, float innerFieldWidth, float innerFieldDepth)
	{
		float wallHeight = 1;
		float outerFieldWidth = 80;
		float outerFieldDepth = 120;

		auto colorDatabase = GlutApp::Get()->GetColorDatabase();
		const physx::PxVec3* outerWallColor = colorDatabase->GetEntry(CRC32_STR("OuterWall")).data;

		physx::PxGeometry* outerWallX = geoFactory->CreateBox(physx::PxVec3(outerFieldWidth / 2, wallHeight / 2, minHeightOffset / 2));
		physx::PxGeometry* outerWallZ = geoFactory->CreateBox(physx::PxVec3(minHeightOffset / 2, wallHeight / 2, outerFieldDepth / 2));

		GameObject* topOuterWall = m_gameObjectFactory->CreateStaticGameObject(physx::PxTransform(physx::PxVec3(0, wallHeight / 2, outerFieldDepth / 2)), GameObject::Layer::Layer_Default);
		shapeCreator->CreateShape(*topOuterWall->GetPhysicsActorPointer(), outerWallX, CRC32_STR("Asphalt"));
		auto& topOuterWallRenderData = topOuterWall->GetRenderData();
		topOuterWallRenderData.SetColour(outerWallColor);
		topOuterWallRenderData.SetLightingDisabled(true);
		AddGameActor(topOuterWall);

		GameObject* bottomOuterWall = m_gameObjectFactory->CreateStaticGameObject(physx::PxTransform(physx::PxVec3(0, wallHeight / 2, -outerFieldDepth / 2)), GameObject::Layer::Layer_Default);
		shapeCreator->CreateShape(*bottomOuterWall->GetPhysicsActorPointer(), outerWallX, CRC32_STR("Asphalt"));
		auto& bottomOuterWallRenderData = bottomOuterWall->GetRenderData();
		bottomOuterWallRenderData.SetColour(outerWallColor);
		bottomOuterWallRenderData.SetLightingDisabled(true);
		AddGameActor(bottomOuterWall);

		GameObject* leftOuterWall = m_gameObjectFactory->CreateStaticGameObject(physx::PxTransform(physx::PxVec3(-outerFieldWidth / 2, wallHeight / 2, 0)), GameObject::Layer::Layer_Default);
		shapeCreator->CreateShape(*leftOuterWall->GetPhysicsActorPointer(), outerWallZ, CRC32_STR("Asphalt"));
		auto& leftOuterWallRenderData = leftOuterWall->GetRenderData();
		leftOuterWallRenderData.SetColour(outerWallColor);
		leftOuterWallRenderData.SetLightingDisabled(true);
		AddGameActor(leftOuterWall);

		GameObject* rightOuterWall = m_gameObjectFactory->CreateStaticGameObject(physx::PxTransform(physx::PxVec3(outerFieldWidth / 2, wallHeight / 2, 0)), GameObject::Layer::Layer_Default);
		shapeCreator->CreateShape(*rightOuterWall->GetPhysicsActorPointer(), outerWallZ, CRC32_STR("Asphalt"));
		auto& rightOuterWallRenderData = rightOuterWall->GetRenderData();
		rightOuterWallRenderData.SetColour(outerWallColor);
		rightOuterWallRenderData.SetLightingDisabled(true);
		AddGameActor(rightOuterWall);
	}

	void PlayGameScene::GameFieldMarking(const PhysicsEngine::ShapeFactory* shapeCreator, const PhysicsEngine::GeometryFactory* geoFactory, float minHeightOffset, float innerFieldWidth, float innerFieldDepth)
	{
		float goalZoneRadius = 16.45f;

		float markingLineWidth = 0.12;
		float markingLineWidthCentral = 0.24;

		physx::PxGeometry* innerFieldMarkingXTrigger = geoFactory->CreateBox(physx::PxVec3(innerFieldWidth / 2, 1.f, markingLineWidth / 2));
		physx::PxGeometry* innerFieldMarkingZTrigger = geoFactory->CreateBox(physx::PxVec3(markingLineWidth / 2, 1.f, innerFieldDepth / 2));

		auto colorDatabase = GlutApp::Get()->GetColorDatabase();
		const physx::PxVec3* whiteColor = colorDatabase->GetEntry(CRC32_STR("White")).data;

		// Gamefield marking
		GameObject* topInnerLineMarking = m_gameObjectFactory->CreateCustomRenderGameObject();
		auto& topLineMarkingRenderData = topInnerLineMarking->GetRenderData();
		topLineMarkingRenderData.SetColour(whiteColor);
		topLineMarkingRenderData.SetLineWidth(markingLineWidth);

		auto topInnerLineMarkingRender = (CustomRenderGameObject*)topInnerLineMarking;
		topInnerLineMarkingRender->SetCustomRenderCallback(*new AppVoidCallbackEntry(topInnerLineMarking, [topInnerLineMarking, minHeightOffset, innerFieldWidth, innerFieldDepth, markingLineWidth]()
			{
				const Renderer* renderer = GlutApp::Get()->GetRenderer();
				const IRender* impl = renderer->GetRenderingImpl();
				const physx::PxTransform transform = physx::PxTransform(physx::PxVec3(0, minHeightOffset - 1, innerFieldDepth / 2));
				impl->DrawBox(transform, physx::PxVec3(innerFieldWidth / 2, 1, markingLineWidth / 2), topInnerLineMarking->GetRenderData());
			}));
		AddGameActor(topInnerLineMarking);

		GameObject* topInnerLineMarkingTrigger = m_gameObjectFactory->CreateStaticGameObject(physx::PxTransform(physx::PxVec3(0, minHeightOffset, innerFieldDepth / 2)), GameObject::Layer::Layer_Default);
		shapeCreator->CreateTrigger(*topInnerLineMarkingTrigger->GetPhysicsActorPointer(), innerFieldMarkingXTrigger);
		RenderData& topInnerLineMarkingTriggerRenderData = topInnerLineMarkingTrigger->GetRenderData();
		topInnerLineMarkingTriggerRenderData.SetIgnoreRender(true);
		AddGameActor(topInnerLineMarkingTrigger);


		GameObject* bottomInnerLineMarking = m_gameObjectFactory->CreateCustomRenderGameObject();
		auto& bottomLineMarkingRenderData = bottomInnerLineMarking->GetRenderData();
		bottomLineMarkingRenderData.SetColour(whiteColor);
		bottomLineMarkingRenderData.SetLineWidth(markingLineWidth);

		// Bottom line
		auto bottomInnerLineMarkingRender = (CustomRenderGameObject*)bottomInnerLineMarking;
		bottomInnerLineMarkingRender->SetCustomRenderCallback(*new AppVoidCallbackEntry(bottomInnerLineMarking, [bottomInnerLineMarking, minHeightOffset, innerFieldWidth, innerFieldDepth, markingLineWidth]()
			{
				const Renderer* renderer = GlutApp::Get()->GetRenderer();
				const IRender* impl = renderer->GetRenderingImpl();
				const physx::PxTransform transform = physx::PxTransform(physx::PxVec3(0, minHeightOffset - 1, -innerFieldDepth / 2), physx::PxIdentity);
				impl->DrawBox(transform, physx::PxVec3(innerFieldWidth / 2, 1, markingLineWidth / 2), bottomInnerLineMarking->GetRenderData());
			}));
		AddGameActor(bottomInnerLineMarking);

		GameObject* bottomInnerLineMarkingTrigger = m_gameObjectFactory->CreateStaticGameObject(physx::PxTransform(physx::PxVec3(0, minHeightOffset, -innerFieldDepth / 2)), GameObject::Layer::Layer_Default);
		shapeCreator->CreateTrigger(*bottomInnerLineMarkingTrigger->GetPhysicsActorPointer(), innerFieldMarkingXTrigger);
		RenderData& bottomInnerLineMarkingTriggerRenderData = bottomInnerLineMarkingTrigger->GetRenderData();
		bottomInnerLineMarkingTriggerRenderData.SetIgnoreRender(true);
		AddGameActor(bottomInnerLineMarkingTrigger);


		// Left line
		GameObject* leftInnerLineMarking = m_gameObjectFactory->CreateCustomRenderGameObject();
		RenderData& leftLineMarkingRenderData = leftInnerLineMarking->GetRenderData();
		leftLineMarkingRenderData.SetColour(whiteColor);
		leftLineMarkingRenderData.SetLineWidth(markingLineWidth);

		auto leftInnerLineMarkingRender = (CustomRenderGameObject*)leftInnerLineMarking;
		leftInnerLineMarkingRender->SetCustomRenderCallback(*new AppVoidCallbackEntry(leftInnerLineMarking, [leftInnerLineMarking, minHeightOffset, innerFieldWidth, innerFieldDepth, markingLineWidth]()
			{
				const Renderer* renderer = GlutApp::Get()->GetRenderer();
				const IRender* impl = renderer->GetRenderingImpl();
				const physx::PxTransform transform = physx::PxTransform(physx::PxVec3(-innerFieldWidth / 2, minHeightOffset - 1, 0));
				impl->DrawBox(transform, physx::PxVec3(markingLineWidth / 2, 1, innerFieldDepth / 2), leftInnerLineMarking->GetRenderData());
			}));
		AddGameActor(leftInnerLineMarking);

		GameObject* leftInnerLineMarkingTrigger = m_gameObjectFactory->CreateStaticGameObject(physx::PxTransform(physx::PxVec3(-innerFieldWidth / 2, minHeightOffset, 0)), GameObject::Layer::Layer_Default);
		shapeCreator->CreateTrigger(*leftInnerLineMarkingTrigger->GetPhysicsActorPointer(), innerFieldMarkingZTrigger);
		RenderData& leftInnerLineMarkingTriggerRenderData = leftInnerLineMarkingTrigger->GetRenderData();
		leftInnerLineMarkingTriggerRenderData.SetIgnoreRender(true);
		AddGameActor(leftInnerLineMarkingTrigger);

		// Right line
		GameObject* rightInnerLineMarking = m_gameObjectFactory->CreateCustomRenderGameObject();
		RenderData& rightLineMarkingRenderData = rightInnerLineMarking->GetRenderData();
		rightLineMarkingRenderData.SetColour(whiteColor);
		rightLineMarkingRenderData.SetLineWidth(markingLineWidth);

		auto rightInnerLineMarkingRender = (CustomRenderGameObject*)rightInnerLineMarking;
		rightInnerLineMarkingRender->SetCustomRenderCallback(*new AppVoidCallbackEntry(rightInnerLineMarking, [rightInnerLineMarking, minHeightOffset, innerFieldWidth, innerFieldDepth, markingLineWidth]()
			{
				const Renderer* renderer = GlutApp::Get()->GetRenderer();
				const IRender* impl = renderer->GetRenderingImpl();
				const physx::PxTransform transform = physx::PxTransform(physx::PxVec3(innerFieldWidth / 2, minHeightOffset - 1, 0));
				impl->DrawBox(transform, physx::PxVec3(markingLineWidth / 2, 1, innerFieldDepth / 2), rightInnerLineMarking->GetRenderData());
			}));
		AddGameActor(rightInnerLineMarking);

		GameObject* rightInnerLineMarkingTrigger = m_gameObjectFactory->CreateStaticGameObject(physx::PxTransform(physx::PxVec3(innerFieldWidth / 2, minHeightOffset, 0)), GameObject::Layer::Layer_Default);
		shapeCreator->CreateTrigger(*rightInnerLineMarkingTrigger->GetPhysicsActorPointer(), innerFieldMarkingZTrigger);
		RenderData& rightInnerLineMarkingTriggerRenderData = rightInnerLineMarkingTrigger->GetRenderData();
		rightInnerLineMarkingTriggerRenderData.SetIgnoreRender(true);
		AddGameActor(rightInnerLineMarkingTrigger);

		// Center line
		GameObject* centerLineMarking = m_gameObjectFactory->CreateCustomRenderGameObject();
		RenderData& centerLineMarkingRenderData = rightInnerLineMarking->GetRenderData();
		centerLineMarkingRenderData.SetColour(whiteColor);
		centerLineMarkingRenderData.SetLineWidth(markingLineWidthCentral);

		auto centerLineMarkingRender = (CustomRenderGameObject*)centerLineMarking;
		centerLineMarkingRender->SetCustomRenderCallback(*new AppVoidCallbackEntry(centerLineMarking, [centerLineMarking, minHeightOffset, innerFieldWidth, innerFieldDepth, markingLineWidthCentral]()
			{
				const Renderer* renderer = GlutApp::Get()->GetRenderer();
				const IRender* impl = renderer->GetRenderingImpl();
				const physx::PxTransform transform = physx::PxTransform(physx::PxVec3(0, minHeightOffset - 1, 0));
				impl->DrawBox(transform, physx::PxVec3(innerFieldWidth / 2, 1, markingLineWidthCentral / 2), centerLineMarking->GetRenderData());
			}));
		AddGameActor(centerLineMarkingRender);

		// Center circle
		GameObject* centerCircle = m_gameObjectFactory->CreateCustomRenderGameObject();
		auto& centerCircleRenderData = centerCircle->GetRenderData();
		centerCircleRenderData.SetColour(whiteColor);
		centerCircleRenderData.SetLineWidth(markingLineWidth);

		auto castedCircle = (CustomRenderGameObject*)centerCircle;
		castedCircle->SetCustomRenderCallback(*new AppVoidCallbackEntry(centerCircle, [centerCircle]()
			{
				const Renderer* renderer = GlutApp::Get()->GetRenderer();
				const IRender* impl = renderer->GetRenderingImpl();
				impl->DrawCircleLine(physx::PxVec3(0, 0.05f, 0), 18.3f, 200, 0, 2 * M_PI, centerCircle->GetRenderData());
			}));
		AddGameActor(centerCircle);

		// Bottom goal circle
		GameObject* bottomGoalCircle = m_gameObjectFactory->CreateCustomRenderGameObject();
		auto& bottomCircleRenderData = bottomGoalCircle->GetRenderData();
		bottomCircleRenderData.SetColour(whiteColor);
		bottomCircleRenderData.SetLineWidth(markingLineWidth);

		auto castedBottomCircle = (CustomRenderGameObject*)bottomGoalCircle;
		castedBottomCircle->SetCustomRenderCallback(*new AppVoidCallbackEntry(bottomGoalCircle, [bottomGoalCircle]()
			{
				const Renderer* renderer = GlutApp::Get()->GetRenderer();
				const IRender* impl = renderer->GetRenderingImpl();
				impl->DrawCircleLine(physx::PxVec3(0, 0.05f, -55), 5.75f, 200, 0, M_PI, bottomGoalCircle->GetRenderData());
			}));
		AddGameActor(bottomGoalCircle);

		// Top goal circle
		GameObject* topGoalCircle = m_gameObjectFactory->CreateCustomRenderGameObject();
		auto& topCircleRenderData = topGoalCircle->GetRenderData();
		topCircleRenderData.SetColour(whiteColor);
		topCircleRenderData.SetLineWidth(markingLineWidth);

		auto castedTopCircle = (CustomRenderGameObject*)topGoalCircle;
		castedTopCircle->SetCustomRenderCallback(*new AppVoidCallbackEntry(topGoalCircle, [topGoalCircle]()
			{
				const Renderer* renderer = GlutApp::Get()->GetRenderer();
				const IRender* impl = renderer->GetRenderingImpl();
				impl->DrawCircleLine(physx::PxVec3(0, 0.05f, 55), 5.75f, 60, M_PI, 2 * M_PI, topGoalCircle->GetRenderData());
			}));
		AddGameActor(topGoalCircle);

		// Top penalty spot
		GameObject* topPenaltySpot = m_gameObjectFactory->CreateCustomRenderGameObject();
		auto& topPenaltySpotRenderData = topPenaltySpot->GetRenderData();
		topPenaltySpotRenderData.SetColour(whiteColor);

		auto castedTopPenaltySpot = (CustomRenderGameObject*)topPenaltySpot;
		castedTopPenaltySpot->SetCustomRenderCallback(*new AppVoidCallbackEntry(topPenaltySpot, [topPenaltySpot]()
			{
				const Renderer* renderer = GlutApp::Get()->GetRenderer();
				const IRender* impl = renderer->GetRenderingImpl();
				impl->DrawCircle(physx::PxVec3(0, 0.05f, 44), 0.25f, 60, 0, 2 * M_PI, topPenaltySpot->GetRenderData());
			}));
		AddGameActor(topPenaltySpot);

		// Bottom penalty spot
		GameObject* bottomPenaltySpot = m_gameObjectFactory->CreateCustomRenderGameObject();
		auto& bottomPenaltySpotRenderData = bottomPenaltySpot->GetRenderData();
		bottomPenaltySpotRenderData.SetColour(whiteColor);

		auto castedBottomPenaltySpot = (CustomRenderGameObject*)bottomPenaltySpot;
		castedBottomPenaltySpot->SetCustomRenderCallback(*new AppVoidCallbackEntry(bottomPenaltySpot, [bottomPenaltySpot]()
			{
				const Renderer* renderer = GlutApp::Get()->GetRenderer();
				const IRender* impl = renderer->GetRenderingImpl();
				impl->DrawCircle(physx::PxVec3(0, 0.05f, -44), 0.25f, 60, 0, 2 * M_PI, bottomPenaltySpot->GetRenderData());
			}));
		AddGameActor(bottomPenaltySpot);

		// Top goal zone
		GameObject* topGoalLineLeftMarking = m_gameObjectFactory->CreateCustomRenderGameObject();
		RenderData& topGoalLineLeftMarkingRenderData = topGoalLineLeftMarking->GetRenderData();
		topGoalLineLeftMarkingRenderData.SetColour(whiteColor);
		topGoalLineLeftMarkingRenderData.SetLineWidth(markingLineWidth);

		auto topGoalLineLeftMarkingRender = (CustomRenderGameObject*)topGoalLineLeftMarking;
		topGoalLineLeftMarkingRender->SetCustomRenderCallback(*new AppVoidCallbackEntry(topGoalLineLeftMarking, [topGoalLineLeftMarking, minHeightOffset, innerFieldWidth, innerFieldDepth, markingLineWidth, goalZoneRadius]()
			{
				const Renderer* renderer = GlutApp::Get()->GetRenderer();
				const IRender* impl = renderer->GetRenderingImpl();
				const physx::PxTransform transform = physx::PxTransform(physx::PxVec3(-goalZoneRadius, minHeightOffset - 1, innerFieldDepth / 2 - goalZoneRadius / 2));
				impl->DrawBox(transform, physx::PxVec3(markingLineWidth / 2, 1, goalZoneRadius / 2), topGoalLineLeftMarking->GetRenderData());
			}));
		AddGameActor(topGoalLineLeftMarking);

		GameObject* topGoalLineRightMarking = m_gameObjectFactory->CreateCustomRenderGameObject();
		RenderData& topGoalLineRightMarkingRenderData = topGoalLineRightMarking->GetRenderData();
		topGoalLineRightMarkingRenderData.SetColour(whiteColor);
		topGoalLineRightMarkingRenderData.SetLineWidth(markingLineWidth);

		auto topGoalLineRightMarkingRender = (CustomRenderGameObject*)topGoalLineRightMarking;
		topGoalLineRightMarkingRender->SetCustomRenderCallback(*new AppVoidCallbackEntry(topGoalLineRightMarking, [topGoalLineRightMarking, minHeightOffset, innerFieldWidth, innerFieldDepth, markingLineWidth, goalZoneRadius]()
			{
				const Renderer* renderer = GlutApp::Get()->GetRenderer();
				const IRender* impl = renderer->GetRenderingImpl();
				const physx::PxTransform transform = physx::PxTransform(physx::PxVec3(goalZoneRadius, minHeightOffset - 1, innerFieldDepth / 2 - goalZoneRadius / 2));
				impl->DrawBox(transform, physx::PxVec3(markingLineWidth / 2, 1, goalZoneRadius / 2), topGoalLineRightMarking->GetRenderData());
			}));
		AddGameActor(topGoalLineRightMarking);

		GameObject* topGoalLineMarking = m_gameObjectFactory->CreateCustomRenderGameObject();
		RenderData& topGoalLineMarkingRenderData = topGoalLineMarking->GetRenderData();
		topGoalLineMarkingRenderData.SetColour(whiteColor);
		topGoalLineMarkingRenderData.SetLineWidth(markingLineWidth);

		auto topGoalLineMarkingRender = (CustomRenderGameObject*)topGoalLineMarking;
		topGoalLineMarkingRender->SetCustomRenderCallback(*new AppVoidCallbackEntry(topGoalLineMarking, [topGoalLineRightMarking, minHeightOffset, innerFieldWidth, innerFieldDepth, markingLineWidth, goalZoneRadius]()
			{
				const Renderer* renderer = GlutApp::Get()->GetRenderer();
				const IRender* impl = renderer->GetRenderingImpl();
				const physx::PxTransform transform = physx::PxTransform(physx::PxVec3(0, minHeightOffset - 1, innerFieldDepth / 2 - goalZoneRadius));
				impl->DrawBox(transform, physx::PxVec3(goalZoneRadius + 0.05, 1, markingLineWidth / 2), topGoalLineRightMarking->GetRenderData());
			}));
		AddGameActor(topGoalLineMarking);

		// Bottom goal zone
		GameObject* bottomGoalLineLeftMarking = m_gameObjectFactory->CreateCustomRenderGameObject();
		RenderData& bottomGoalLineLeftMarkingRenderData = bottomGoalLineLeftMarking->GetRenderData();
		bottomGoalLineLeftMarkingRenderData.SetColour(whiteColor);
		bottomGoalLineLeftMarkingRenderData.SetLineWidth(markingLineWidth);

		auto bottomGoalLineLeftMarkingRender = (CustomRenderGameObject*)bottomGoalLineLeftMarking;
		bottomGoalLineLeftMarkingRender->SetCustomRenderCallback(*new AppVoidCallbackEntry(bottomGoalLineLeftMarking, [bottomGoalLineLeftMarking, minHeightOffset, innerFieldWidth, innerFieldDepth, markingLineWidth, goalZoneRadius]()
			{
				const Renderer* renderer = GlutApp::Get()->GetRenderer();
				const IRender* impl = renderer->GetRenderingImpl();
				const physx::PxTransform transform = physx::PxTransform(physx::PxVec3(-goalZoneRadius, minHeightOffset - 1, -innerFieldDepth / 2 + goalZoneRadius / 2));
				impl->DrawBox(transform, physx::PxVec3(markingLineWidth / 2, 1, goalZoneRadius / 2), bottomGoalLineLeftMarking->GetRenderData());
			}));
		AddGameActor(bottomGoalLineLeftMarking);

		GameObject* bottomGoalLineRightMarking = m_gameObjectFactory->CreateCustomRenderGameObject();
		RenderData& bottomGoalLineRightMarkingRenderData = bottomGoalLineRightMarking->GetRenderData();
		bottomGoalLineRightMarkingRenderData.SetColour(whiteColor);
		bottomGoalLineRightMarkingRenderData.SetLineWidth(markingLineWidth);

		auto bottomGoalLineRightMarkingRender = (CustomRenderGameObject*)bottomGoalLineRightMarking;
		bottomGoalLineRightMarkingRender->SetCustomRenderCallback(*new AppVoidCallbackEntry(bottomGoalLineRightMarking, [bottomGoalLineRightMarking, minHeightOffset, innerFieldWidth, innerFieldDepth, markingLineWidth, goalZoneRadius]()
			{
				const Renderer* renderer = GlutApp::Get()->GetRenderer();
				const IRender* impl = renderer->GetRenderingImpl();
				const physx::PxTransform transform = physx::PxTransform(physx::PxVec3(goalZoneRadius, minHeightOffset - 1, -innerFieldDepth / 2 + goalZoneRadius / 2));
				impl->DrawBox(transform, physx::PxVec3(markingLineWidth / 2, 1, goalZoneRadius / 2), bottomGoalLineRightMarking->GetRenderData());
			}));
		AddGameActor(bottomGoalLineRightMarking);

		GameObject* bottomGoalLineMarking = m_gameObjectFactory->CreateCustomRenderGameObject();
		RenderData& bottomGoalLineMarkingRenderData = bottomGoalLineMarking->GetRenderData();
		bottomGoalLineMarkingRenderData.SetColour(whiteColor);
		bottomGoalLineMarkingRenderData.SetLineWidth(markingLineWidth);

		auto bottomGoalLineMarkingRender = (CustomRenderGameObject*)bottomGoalLineMarking;
		bottomGoalLineMarkingRender->SetCustomRenderCallback(*new AppVoidCallbackEntry(bottomGoalLineMarking, [bottomGoalLineMarking, minHeightOffset, innerFieldWidth, innerFieldDepth, markingLineWidth, goalZoneRadius]()
			{
				const Renderer* renderer = GlutApp::Get()->GetRenderer();
				const IRender* impl = renderer->GetRenderingImpl();
				const physx::PxTransform transform = physx::PxTransform(physx::PxVec3(0, minHeightOffset - 1, -innerFieldDepth / 2 + goalZoneRadius));
				impl->DrawBox(transform, physx::PxVec3(goalZoneRadius + 0.05, 1, markingLineWidth / 2), bottomGoalLineMarking->GetRenderData());
			}));
		AddGameActor(bottomGoalLineMarking);
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
