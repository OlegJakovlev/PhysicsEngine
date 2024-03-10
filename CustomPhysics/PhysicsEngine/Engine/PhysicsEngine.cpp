#include "PhysicsEngine.h"

namespace PhysicsEngine
{
	PhysicsEngine* PhysicsEngine::s_instance = nullptr;
	std::once_flag PhysicsEngine::s_initialized;

	PhysicsEngine* PhysicsEngine::Instance()
	{
		std::call_once(s_initialized, []()
		{
			s_instance = new PhysicsEngine();
		});

		return s_instance;
	}

	void PhysicsEngine::Release()
	{
		m_sceneManager->Release();
		delete m_sceneManager;
		m_sceneManager = nullptr;

		m_vehicleSDK->Release();
		delete m_vehicleSDK;
		m_vehicleSDK = nullptr;

		m_cooking->Release();
		delete m_cooking;
		m_cooking = nullptr;

		m_physics->Release();
		delete m_physics;
		m_physics = nullptr;

		m_visualDebugger->Release();
		delete m_visualDebugger;
		m_visualDebugger = nullptr;

		m_foundation->Release();
		delete m_foundation;
		m_foundation = nullptr;

		// Materials are deallocated by m_physics
		delete m_materialDatabase;
		m_materialDatabase = nullptr;
	}

	bool PhysicsEngine::Init()
	{
		m_foundation = new Foundation();
		if (!m_foundation->Init())
		{
			printf("Foundation creation failed!\n");
			return false;
		}

		m_visualDebugger = new VisualDebugger();
		m_visualDebugger->Init(m_foundation->GetFoundationService());

		m_physics = new Physics();
		if (!m_physics->Init(m_foundation->GetFoundationService(), m_visualDebugger->GetPVDService()))
		{
			printf("Physics creation failed!\n");
			return false;
		}

		m_dispatcher = new Dispatcher();
		if (!m_dispatcher->Init())
		{
			printf("Dispatcher creation failed!\n");
			return false;
		}

		m_cooking = new Cooking();
		if (!m_cooking->Init(m_foundation->GetFoundationService(), m_physics->GetPhysics()))
		{
			printf("Cooking creation failed!\n");
			return false;
		}

		/* Is not supported because of linker and physx :(
		m_taskManager = new TaskManager();
		if (!m_taskManager->Init(m_foundation->GetFoundationService(), m_dispatcher))
		{
			return false;
		}
		*/

		m_materialDatabase = new MaterialDatabase();
		if (!m_materialDatabase->Init(m_physics->GetPhysics()))
		{
			printf("MaterialDatabase creation failed!\n");
			return false;
		}

		m_sceneManager = new SceneManager();

		m_actorFactory = new ActorFactory();
		if (!m_actorFactory->Init(m_physics->GetPhysics()))
		{
			printf("ActorFactory creation failed!\n");
			return false;
		}

		m_geoFactory = new GeometryFactory();
		if (!m_geoFactory->Init(m_physics->GetPhysics(), m_cooking->GetCooking()))
		{
			printf("GeometryFactory creation failed!\n");
			return false;
		}

		m_shapeCreator = new ShapeCreator();
		if (!m_shapeCreator->Init(m_materialDatabase))
		{
			printf("ShapeCreator creation failed!\n");
			return false;
		}

		m_vehicleSDK = new VehicleSDKWrapper();
		if (!m_vehicleSDK->Init(m_physics->GetPhysics()))
		{
			printf("VehicleSDKWrapper creation failed!\n");
			return false;
		}

		return true;
	}

	bool PhysicsEngine::PostInit()
	{
		if (!m_sceneManager->PostInit(m_physics->GetPhysics(), m_dispatcher))
		{
			printf("SceneManager creation failed!\n");
			return false;
		}

		return true;
	}

	void PhysicsEngine::Update(float dt)
	{
		// TODO: Think about if we actually need these frame allocators
		g_singleFrameAllocator.Clear();
		g_doubleBufferAllocator.SwapBuffers();

		// Do subticks for better stability
		// Info from: https://gameworksdocs.nvidia.com/PhysX/3.4/PhysXGuide/Manual/Simulation.html#substepping)
		for (int i = 0; i < k_substeps; i++)
		{
			//std::printf("PhysicsEngine::Update(%f), Substep: %i\n", dt, i);
			m_sceneManager->Update(dt / k_substeps);
		}
	}

	void PhysicsEngine::Sync()
	{
		m_sceneManager->Sync();
	}

	const SceneManager* PhysicsEngine::GetSceneManager() const
	{
		return m_sceneManager;
	}

	const ActorFactory* PhysicsEngine::GetActorFactory() const
	{
		return m_actorFactory;
	}

	const GeometryFactory* PhysicsEngine::GetGeometryFactory() const
	{
		return m_geoFactory;
	}

	const MaterialDatabase* PhysicsEngine::GetMaterialDatabase() const
	{
		return m_materialDatabase;
	}

	const ShapeCreator* PhysicsEngine::GetShapeCreator() const
	{
		return m_shapeCreator;
	}
}
