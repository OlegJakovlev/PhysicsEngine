#pragma once

#include "../GlobalDefine.h"
#include "../API/IPhysicsEngine.h"
#include "Services/Foundation.h"
#include "Services/Physics.h"
#include "Services/Dispatcher.h"
#include "SceneManagement/SceneManager.h"
#include "Services/TaskManager.h"
#include "../Databases/MaterialDatabase.h"
#include "../Allocators/SingleFrameAllocator.h"
#include "../Allocators/DoubleBufferAllocator.h"
#include "Actors/ActorFactory.h"
#include "Actors/GeometryFactory.h"
#include "../Utility/ShapeCreator.h"
#include "../Debug/VisualDebugger.h"

namespace PhysicsEngine
{
	// Composition Root
	class PhysicsEngine : public IPhysicsEngine
	{
	private:
		static PhysicsEngine* s_instance;
		static std::once_flag s_initialized;

		static constexpr float k_deltaTime = 1.0f / 60.0f;
		static constexpr int k_substeps = 4;

		// Services
		Foundation* m_foundation;
		Physics* m_physics;
		Dispatcher* m_dispatcher;
		SceneManager* m_sceneManager;
		TaskManager* m_taskManager;
		VisualDebugger* m_visualDebugger;

		// Factories and databases
		ActorFactory* m_actorFactory;
		GeometryFactory* m_geoFactory;
		ShapeCreator* m_shapeCreator;
		MaterialDatabase* m_materialDatabase;

		// Local variables
		SingleFrameAllocator g_singleFrameAllocator;
		DoubleBufferAllocator g_doubleBufferAllocator;

		PhysicsEngine() = default;
		PhysicsEngine(const PhysicsEngine&) = delete;
		PhysicsEngine& operator=(const PhysicsEngine&) = delete;

	public:
		static PhysicsEngine* Instance();
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
}