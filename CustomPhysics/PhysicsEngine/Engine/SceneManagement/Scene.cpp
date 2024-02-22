#include "Scene.h"
#include "../../Utility/CLZ.h"

namespace PhysicsEngine
{
	Scene::SceneConfiguration::SceneConfiguration()
	{
		m_collisionFilter = new CollisionFilter();
		m_gravity = physx::PxVec3();
		m_enableDemo = false;
	}

	Scene::SceneConfiguration::SceneConfiguration(const SceneConfiguration& sceneConfig)
	{
		m_collisionFilter = sceneConfig.m_collisionFilter;
		m_gravity = sceneConfig.m_gravity;
		m_enableDemo = sceneConfig.m_enableDemo;
	}

	Scene::SceneConfiguration::~SceneConfiguration()
	{
		delete m_collisionFilter;
	}

	Scene::Scene(uint32_t id) : m_id(id)
	{
		m_staticActors = new Actor* [k_maxStaticActors];
		for (std::uint32_t i = 0; i < k_maxStaticActors; i++)
		{
			m_staticActors[i] = nullptr;
		}

		m_dynamicActors = new Actor* [k_maxDynamicActors];
		for (std::uint32_t i = 0; i < k_maxDynamicActors; i++)
		{
			m_dynamicActors[i] = nullptr;
		}
	}

	bool Scene::Init(const SceneConfiguration* configuration)
	{
		m_configuration = configuration;

		m_tracker = new EventTracker();
		if (!m_tracker->Init(m_id, configuration->m_enableDemo))
		{
			std::printf("EventTracker init failed!\n");
			return false;
		}

		return true;
	}

	bool Scene::PostInit(const physx::PxPhysics* physxObject,
						 const physx::PxCpuDispatcher* dispatcherObject)
	{
		physx::PxSceneDesc sceneDesc(physxObject->getTolerancesScale());
		sceneDesc.gravity = m_configuration->m_gravity;
		sceneDesc.filterShader = m_configuration->m_collisionFilter->GetFilter();
		sceneDesc.cpuDispatcher = const_cast<physx::PxCpuDispatcher*>(dispatcherObject);

		sceneDesc.kineKineFilteringMode = physx::PxPairFilteringMode::eKEEP; // Kinematic-Kinematic contacts
		sceneDesc.staticKineFilteringMode = physx::PxPairFilteringMode::eKEEP; // Static-Kinematic contacts
		//sceneDesc.simulationEventCallback = 

		/*
			There may be a performance penalty for enabling the Active Actor Notification, hence this flag should
			only be enabled if the application intends to use the feature.
		*/
		sceneDesc.flags |= physx::PxSceneFlag::eENABLE_ACTIVE_ACTORS;

		/*
			This flag provides an additional level of determinism that guarantees that the simulation will not change if additional actors are added to the simulation, provided those actors do not interfere
			with the existing actors in the scene. Determinism is only guaranteed if the actors are inserted in a consistent order each run in a newly-created scene and simulated using a consistent time-stepping
			scheme.
		*/
		sceneDesc.flags |= physx::PxSceneFlag::eENABLE_ENHANCED_DETERMINISM;

		// Multithreading locks
		// sceneDesc.flags |= physx::PxSceneFlag::eREQUIRE_RW_LOCK;

		sceneDesc.flags |= physx::PxSceneFlag::eENABLE_CCD;

		m_physxScene = const_cast<physx::PxPhysics*>(physxObject)->createScene(sceneDesc);

#ifdef REMOTE_VISUAL_DEBUG
		// Transmits streams to visual debugger 
		physx::PxPvdSceneClient* pvdClient = m_physxScene->getScenePvdClient();
		if (pvdClient)
		{
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, false);
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, false);

			m_physxScene->setVisualizationParameter(physx::PxVisualizationParameter::eSCALE, 1.0f);

			// Joints
			m_physxScene->setVisualizationParameter(physx::PxVisualizationParameter::eJOINT_LOCAL_FRAMES, 1.0f);
			m_physxScene->setVisualizationParameter(physx::PxVisualizationParameter::eJOINT_LIMITS, 1.0f);

			// Collisions
			m_physxScene->setVisualizationParameter(physx::PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);

			// Cloth
			m_physxScene->setVisualizationParameter(physx::PxVisualizationParameter::eCLOTH_HORIZONTAL, 1.0f);
			m_physxScene->setVisualizationParameter(physx::PxVisualizationParameter::eCLOTH_VERTICAL, 1.0f);
			m_physxScene->setVisualizationParameter(physx::PxVisualizationParameter::eCLOTH_BENDING, 1.0f);
			m_physxScene->setVisualizationParameter(physx::PxVisualizationParameter::eCLOTH_SHEARING, 1.0f);
		}
#endif

		for (size_t i = 0; i < m_staticActorCount; i++)
		{
			SetupActorFilter(m_staticActors[i]);
			RegisterActor(m_staticActors[i]);
		}

		for (size_t i = 0; i < m_dynamicActorCount; i++)
		{
			SetupActorFilter(m_dynamicActors[i]);
			RegisterActor(m_dynamicActors[i]);
		}

		m_state = State::RUNNING;

		return true;
	}

	void Scene::SetupActorFilter(const Actor* actor)
	{
		CollisionFilter::FilterGroup collisionLayer = actor->GetCollisionLayer();

		// Count leading zeros and invert so we get an layer id
		uint32_t collisionIndex = CLZ::CLZ1(~(uint32_t) collisionLayer);

		physx::PxRigidActor* rigidPxActor = (physx::PxRigidActor*) const_cast<physx::PxActor*>(actor->GetCurrentPhysxActor());

		physx::PxU32 numShapes = rigidPxActor->getNbShapes();
		physx::PxShape** shapes = new physx::PxShape * [numShapes];
		rigidPxActor->getShapes(shapes, numShapes, 0);

		physx::PxFilterData filterData;
		for (physx::PxU32 i = 0; i < numShapes; ++i)
		{
			filterData = shapes[i]->getSimulationFilterData();
			filterData.word0 = (uint32_t) collisionLayer;
			filterData.word1 = m_configuration->m_collisionFilter->GetCollisionMask(collisionIndex);
			shapes[i]->setSimulationFilterData(filterData);
		}

		delete[] shapes;
	}

	void Scene::Update(float dt)
	{
		if (m_state != State::RUNNING) return;

		// When this call returns, the simulation step has begun in a separate thread
		m_physxScene->simulate(dt);

		// Wait until simulation completes
		m_physxScene->fetchResults(true);
	}

	void Scene::Release()
	{
		m_physxScene->release();
		m_physxScene = nullptr;

		// TODO: Release staticActors and dynamic actors memory?

		m_state = State::UNLOADED;
	}

	void Scene::RegisterActor(const Actor* actor)
	{
		m_physxScene->addActor(*const_cast<physx::PxActor*>(actor->GetCurrentPhysxActor()));
		m_tracker->RegisterAddActorEvent(actor);
	}

	void Scene::StaticSync(Scene* sourceScene)
	{
		std::unique_lock<std::mutex> lock(m_mutex);

		for (uint32_t i = 0; i < m_staticActorCount; i++)
		{
			m_staticActors[i]->Release();
			delete m_staticActors[i];
		}

		m_staticActorCount = 0;
#ifdef PHYSICS_DEBUG_MODE
		m_staticActorsDebug.clear();
#endif

		for (uint32_t i = 0; i < sourceScene->m_staticActorCount; i++)
		{
			if (sourceScene->m_engineScene == nullptr)
			{
				m_staticActors[i] = sourceScene->m_staticActors[i]->CloneToRender();
			}
			else
			{
				m_staticActors[i] = sourceScene->m_staticActors[i]->CloneToPhysics();
			}

			AddActor((StaticActor*) m_staticActors[i]);
		}
	}

	void Scene::DynamicSync(Scene* sourceScene)
	{
		std::unique_lock<std::mutex> lock(m_mutex);

		for (uint32_t i = 0; i < m_dynamicActorCount; i++)
		{
			m_dynamicActors[i]->Release();
			delete m_dynamicActors[i];
		}

		m_dynamicActorCount = 0;
#ifdef PHYSICS_DEBUG_MODE
		m_dynamicActorsDebug.clear();
#endif

		for (uint32_t i = 0; i < sourceScene->m_dynamicActorCount; i++)
		{
			if (sourceScene->m_engineScene == nullptr)
			{
				m_dynamicActors[i] = sourceScene->m_dynamicActors[i]->CloneToRender();
			}
			else
			{
				m_dynamicActors[i] = sourceScene->m_dynamicActors[i]->CloneToPhysics();
			}

			AddActor((DynamicActor*) m_dynamicActors[i]);
		}
	}

	void Scene::Lock()
	{
		m_mutex.lock();
	}

	void Scene::Unlock()
	{
		m_mutex.unlock();
	}

	void Scene::LinkEngineScene(void* gameScenePointer)
	{
		m_engineScene = gameScenePointer;
	}

	void Scene::AddActor(StaticActor* actor)
	{
#ifdef PHYSICS_DEBUG_MODE
		m_staticActorsDebug.push_back(actor);
#endif
		
		m_staticActors[m_staticActorCount] = actor;
		m_staticActorCount++;

		if (m_state == State::RUNNING)
		{
			printf("Added static actor in runtime! Warning: Very costly operation! \n");
			RegisterActor(actor);
		}
	}

	void Scene::AddActor(DynamicActor* actor)
	{
#ifdef PHYSICS_DEBUG_MODE
		m_dynamicActorsDebug.push_back(actor);
#endif

		m_dynamicActors[m_dynamicActorCount] = actor;
		m_dynamicActorCount++;

		if (m_state == State::RUNNING)
		{
			RegisterActor(actor);
		}
	}

	const Actor** Scene::GetStaticActors() const
	{
		return const_cast<const Actor**>(m_staticActors);
	}

	const uint32_t Scene::GetStaticActorCount() const
	{
		return m_staticActorCount;
	}

	const Actor** Scene::GetDynamicActors() const
	{
		return const_cast<const Actor**>(m_dynamicActors);
	}

	const uint32_t Scene::GetDynamicActorCount() const
	{
		return m_dynamicActorCount;
	}

	const physx::PxScene* Scene::GetPhysxScene() const
	{
		return m_physxScene;
	}

	const physx::PxRenderBuffer& Scene::GetRenderBuffer() const
	{
		return m_physxScene->getRenderBuffer();
	}
}
