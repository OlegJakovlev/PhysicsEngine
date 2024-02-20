#include "PhysicsEngine.h"

namespace PhysicsEngine
{
	void PhysicsEngine::Release()
	{
		m_sceneManager->Release();
		delete m_sceneManager;
		m_sceneManager = nullptr;

		m_physics->Release();
		delete m_physics;
		m_physics = nullptr;

		m_foundation->Release();
		delete m_foundation;
		m_foundation = nullptr;

#ifdef REMOTE_VISUAL_DEBUG
		m_visualDebuger->Release();
		delete m_visualDebuger;
		m_visualDebuger = nullptr;
#endif

		// Materials are deallocated by m_physics
		delete m_materialDatabase;
		m_materialDatabase = nullptr;
	}

	bool PhysicsEngine::Init()
	{
		m_foundation = new Foundation();
		if (!m_foundation->Init())
		{
			printf("Foundation creation failed!");
			return false;
		}

#ifdef REMOTE_VISUAL_DEBUG
		m_visualDebuger = new VisualDebugger();
		m_visualDebuger->Init(m_foundation->GetFoundationService());
#endif

		m_physics = new Physics();
		if (!m_physics->Init(m_foundation->GetFoundationService(), m_visualDebuger ? m_visualDebuger->GetPVDService() : nullptr))
		{
			printf("Physics creation failed!");
			return false;
		}

		m_dispatcher = new Dispatcher();
		if (!m_dispatcher->Init())
		{
			printf("Dispatcher creation failed!");
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
			printf("MaterialDatabase creation failed!");
			return false;
		}

		m_sceneManager = new SceneManager();

		m_actorFactory = new ActorFactory();
		if (!m_actorFactory->Init(m_physics->GetPhysics()))
		{
			printf("ActorFactory creation failed!");
			return false;
		}

		m_geoFactory = new GeometryFactory();

		m_shapeCreator = new ShapeCreator();
		if (!m_shapeCreator->Init(m_materialDatabase))
		{
			printf("ShapeCreator creation failed!");
			return false;
		}

		return true;
	}

	bool PhysicsEngine::PostInit()
	{
		if (!m_sceneManager->PostInit(m_physics->GetPhysics(), m_dispatcher->GetCPU()))
		{
			printf("SceneManager creation failed!");
			return false;
		}
	}

	void PhysicsEngine::Update(float dt)
	{
		// TODO: Think about if we actually need these frame allocators
		g_singleFrameAllocator.Clear();
		g_doubleBufferAllocator.SwapBuffers();

		// Do subticks for better stability
		// Info from:https://gameworksdocs.nvidia.com/PhysX/3.4/PhysXGuide/Manual/Simulation.html#substepping)
		for (int i = 0; i < k_substeps; i++)
		{
			//std::printf("PhysicsEngine::Update(%f), Substep: %i\n", dt, i);
			m_sceneManager->Update(dt / k_substeps);
		}
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
