#pragma once

#include "../GlobalDefine.h"
#include "../API/IPhysicsEngine.h"
#include "Foundation.h"
#include "Physics.h"
#include "Dispatcher.h"
#include "SceneManager.h"
#include "TaskManager.h"
#include "../Databases/MaterialDatabase.h"
#include "../Allocators/SingleFrameAllocator.h"
#include "../Allocators/DoubleBufferAllocator.h"

#ifdef REMOTE_VISUAL_DEBUG
#include "../Debug/VisualDebugger.h"
#endif
#include "Actors/ActorFactory.h"
#include "Actors/GeometryFactory.h"
#include "../Utility/ShapeCreator.h"

// Composition Root
class PhysicsEngine : public IPhysicsEngine
{
private:
	static constexpr float k_deltaTime = 1.0f / 60.0f;
	static constexpr int k_substeps = 4;

	// Services
	Foundation* m_foundation;
	Physics* m_physics;
	Dispatcher* m_dispatcher;
	SceneManager* m_sceneManager;
	TaskManager* m_taskManager;
#ifdef REMOTE_VISUAL_DEBUG
	VisualDebugger* m_visualDebuger;
#endif

	// Factories and databases
	ActorFactory* m_actorFactory;
	GeometryFactory* m_geoFactory;
	ShapeCreator* m_shapeCreator;
	MaterialDatabase* m_materialDatabase;

	// Local variables
	SingleFrameAllocator g_singleFrameAllocator;
	DoubleBufferAllocator g_doubleBufferAllocator;

#ifdef DEBUG_MODE
	void RunUpdate();
#endif

public:
	void Release();

	bool Init() override;
	bool PostInit() override;
	void Update(float dt) override;

	// TODO: API Exposed
	const SceneManager* GetSceneManager() const;
	const ActorFactory* GetActorFactory() const;
	const GeometryFactory* GetGeometryFactory() const;
	const ShapeCreator* GetShapeCreator() const;
	const MaterialDatabase* GetMaterialDatabase() const;
};