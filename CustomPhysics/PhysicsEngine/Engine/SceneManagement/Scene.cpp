#include "Scene.h"
#include "../../Utility/CLZ.h"
#include "../Physics/CustomSimulationEventCallback.h"
#include "../Physics/VehicleSceneQueryData.h"
#include "../Physics/VehicleSceneQueryResults.h"
#include "../Types/SurfaceType.h"

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
		m_staticActorCount = 0;
		m_staticActors = new Actor* [k_maxStaticActors];
		for (std::uint32_t i = 0; i < k_maxStaticActors; i++)
		{
			m_staticActors[i] = nullptr;
		}

		m_dynamicActorCount = 0;
		m_dynamicActors = new Actor* [k_maxDynamicActors];
		for (std::uint32_t i = 0; i < k_maxDynamicActors; i++)
		{
			m_dynamicActors[i] = nullptr;
		}

		m_clothActorCount = 0;
		m_clothActors = new Actor* [k_maxClothActors];
		for (std::uint32_t i = 0; i < k_maxClothActors; i++)
		{
			m_clothActors[i] = nullptr;
		}

		m_vehicleActorCount = 0;
		m_vehicleActors = new Actor* [k_maxVehicleActors];
		for (std::uint32_t i = 0; i < k_maxVehicleActors; i++)
		{
			m_vehicleActors[i] = nullptr;
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
						 const Dispatcher* dispatcherObject)
	{
		physx::PxSceneDesc sceneDesc(physxObject->getTolerancesScale());
		sceneDesc.gravity = m_configuration->m_gravity;
		sceneDesc.filterShader = m_configuration->m_collisionFilter->GetSimFilter();
		sceneDesc.cpuDispatcher = const_cast<physx::PxCpuDispatcher*>(dispatcherObject->GetCPU());
		sceneDesc.gpuDispatcher = const_cast<physx::PxGpuDispatcher*>(dispatcherObject->GetGPU());

		sceneDesc.kineKineFilteringMode = physx::PxPairFilteringMode::eKEEP; // Kinematic-Kinematic contacts
		sceneDesc.staticKineFilteringMode = physx::PxPairFilteringMode::eKEEP; // Static-Kinematic contacts
		sceneDesc.simulationEventCallback = new CustomSimulationEventCallback();

#if ENABLE_CUDA
		sceneDesc.broadPhaseType = physx::PxBroadPhaseType::eGPU;
#endif

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

		// Persistent Contact Manifold (PCM)
		// GJK-based distance collision detection system
		sceneDesc.flags |= physx::PxSceneFlag::eENABLE_PCM;

#if ENABLE_CUDA
		sceneDesc.flags |= physx::PxSceneFlag::eENABLE_GPU_DYNAMICS;
#endif

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

		for (size_t i = 0; i < m_clothActorCount; i++)
		{
			SetupActorFilter(m_clothActors[i]);
			RegisterActor(m_clothActors[i]);
		}

		for (size_t i = 0; i < m_vehicleActorCount; i++)
		{
			SetupActorFilter(m_vehicleActors[i]);
			RegisterActor(m_vehicleActors[i]);
		}

		m_state = State::RUNNING;

		return true;
	}

	void Scene::SetupActorFilter(const Actor* actor)
	{
		physx::PxActor* castedPhysxActor = const_cast<physx::PxActor*>(actor->GetCurrentPhysxActor());
		
		if (castedPhysxActor->is<physx::PxRigidActor>())
		{
			physx::PxRigidActor* rigidPxActor = (physx::PxRigidActor*) castedPhysxActor;

			bool isVehicle = actor->GetType() == ActorType::Vehicle;

			physx::PxU32 numShapes = rigidPxActor->getNbShapes();
			physx::PxShape** shapes = new physx::PxShape * [numShapes];
			rigidPxActor->getShapes(shapes, numShapes, 0);

			physx::PxFilterData filterData;

			for (physx::PxU32 i = 0; i < numShapes; ++i)
			{
				filterData.word0 = (uint32_t) actor->GetCollisionLayer();
				filterData.word1 = (uint32_t) m_configuration->m_collisionFilter->GetCollisionMask(actor->GetCollisionLayerIndex());

				if (isVehicle)
				{
					filterData.word3 = SurfaceType::UNDRIVABLE_SURFACE;
				}

				shapes[i]->setSimulationFilterData(filterData);
			}

			delete[] shapes;
			return;
		}

		if (castedPhysxActor->is<physx::PxCloth>())
		{
			physx::PxCloth* clothPxActor = (physx::PxCloth*) castedPhysxActor;

			physx::PxFilterData filterData;
			filterData.word0 = (uint32_t)actor->GetCollisionLayer();
			filterData.word1 = (uint32_t) m_configuration->m_collisionFilter->GetCollisionMask(actor->GetCollisionLayerIndex());

			clothPxActor->setSimulationFilterData(filterData);
		}
	}

	void Scene::Prepare()
	{
		if (m_state != State::RUNNING) return;

		VehicleSceneQueryData* a = VehicleSceneQueryData::Create(2);
		VehicleSceneQueryResults* b = VehicleSceneQueryResults::Create(2);

		/*
		physx::PxVehicleDrivableSurfaceToTireFrictionPairs* m_surfaceTirePairs;
		m_surfaceTirePairs->setup(MAX_NUM_TIRE_TYPES, MAX_NUM_SURFACE_TYPES, drivableSurfaceMaterials, drivableSurfaceTypes);
		for (PxU32 i = 0; i < MAX_NUM_SURFACE_TYPES; i++)
		{
			for (PxU32 j = 0; j < MAX_NUM_TIRE_TYPES; j++)
			{
				mSurfaceTirePairs->setTypePairFriction(i, j, TireFrictionMultipliers::getValue(i, j));
			}
		}
		*/
	}

	void Scene::Update(float dt)
	{
		if (m_state != State::RUNNING) return;

		//physx::PxVehicleUpdates(dt, m_physxScene->getGravity(), m_frictionPairs, 1, m_vehicles, NULL);

		// When this call returns, the simulation step has begun in a separate thread
		m_physxScene->simulate(dt);

		// Wait until simulation completes
		m_physxScene->fetchResults(true);
	}

	void Scene::Release()
	{
		m_tracker->Release();
		delete m_tracker;

		for (uint32_t i = 0; i < m_staticActorCount; i++)
		{
			m_staticActors[i]->Release();
			delete m_staticActors[i];
		}

		for (uint32_t i = 0; i < m_dynamicActorCount; i++)
		{
			m_dynamicActors[i]->Release();
			delete m_dynamicActors[i];
		}

		for (uint32_t i = 0; i < m_clothActorCount; i++)
		{
			m_clothActors[i]->Release();
			delete m_clothActors[i];
		}

		m_physxScene->release();
		m_physxScene = nullptr;

		m_state = State::UNLOADED;
	}

	void Scene::RegisterActor(const Actor* actor)
	{
		m_physxScene->addActor(*const_cast<physx::PxActor*>(actor->GetCurrentPhysxActor()));
		m_tracker->RegisterAddActorEvent(actor);
	}

	void Scene::Sync(Scene* sourceScene, SyncState syncState)
	{
		if (syncState & SyncState::STATIC)
		{
			StaticSync(sourceScene);
		}

		if (syncState & SyncState::DYNAMIC)
		{
			DynamicSync(sourceScene);
		}

		if (syncState & SyncState::CLOTH)
		{
			ClothSync(sourceScene);
		}

		if (syncState & SyncState::VEHICLE)
		{
			VehicleSync(sourceScene);
		}

		if (syncState & SyncState::JOINT)
		{
			JointSync(sourceScene);
		}
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

			AddActorInternal((StaticActor*) m_staticActors[i]);
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

			AddActorInternal((DynamicActor*) m_dynamicActors[i]);
		}
	}

	void Scene::ClothSync(Scene* sourceScene)
	{
		std::unique_lock<std::mutex> lock(m_mutex);

		for (uint32_t i = 0; i < m_clothActorCount; i++)
		{
			m_clothActors[i]->Release();
			delete m_clothActors[i];
		}

		m_clothActorCount = 0;
#ifdef PHYSICS_DEBUG_MODE
		m_clothActorsDebug.clear();
#endif

		for (uint32_t i = 0; i < sourceScene->m_clothActorCount; i++)
		{
			if (sourceScene->m_engineScene == nullptr)
			{
				m_clothActors[i] = sourceScene->m_clothActors[i]->CloneToRender();
			}
			else
			{
				m_clothActors[i] = sourceScene->m_clothActors[i]->CloneToPhysics();
			}

			if (m_clothActors[i])
			{
				AddActorInternal((ClothActor*) m_clothActors[i]);
			}
		}
	}

	void Scene::VehicleSync(Scene* sourceScene)
	{
		std::unique_lock<std::mutex> lock(m_mutex);

		for (uint32_t i = 0; i < m_vehicleActorCount; i++)
		{
			m_vehicleActors[i]->Release();
			delete m_vehicleActors[i];
		}

		m_vehicleActorCount = 0;
#ifdef PHYSICS_DEBUG_MODE
		m_vehicleActorsDebug.clear();
#endif

		for (uint32_t i = 0; i < sourceScene->m_vehicleActorCount; i++)
		{
			if (sourceScene->m_engineScene == nullptr)
			{
				m_vehicleActors[i] = sourceScene->m_vehicleActors[i]->CloneToRender();
			}
			else
			{
				m_vehicleActors[i] = sourceScene->m_vehicleActors[i]->CloneToPhysics();
			}

			if (m_vehicleActors[i])
			{
				AddActorInternal((VehicleActor*) m_vehicleActors[i]);
			}
		}
	}

	void Scene::JointSync(Scene* sourceScene)
	{
		std::unique_lock<std::mutex> lock(m_mutex);

		for (uint32_t i = 0; i < m_jointActorCount; i++)
		{
			m_jointActors[i]->Release();
			delete m_jointActors[i];
		}

		m_jointActorCount = 0;
#ifdef PHYSICS_DEBUG_MODE
		m_jointActorsDebug.clear();
#endif

		for (uint32_t i = 0; i < sourceScene->m_jointActorCount; i++)
		{
			if (sourceScene->m_engineScene == nullptr)
			{
				m_jointActors[i] = sourceScene->m_jointActors[i]->CloneToRender();
			}
			else
			{
				m_jointActors[i] = sourceScene->m_jointActors[i]->CloneToPhysics();
			}

			if (m_jointActors[i])
			{
				AddActorInternal((JointActor*)m_jointActors[i]);
			}
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

	void Scene::AddActorInternal(StaticActor* actor)
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

	void Scene::AddActorInternal(DynamicActor* actor)
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

	void Scene::AddActorInternal(ClothActor* actor)
	{
#ifdef PHYSICS_DEBUG_MODE
		m_clothActorsDebug.push_back(actor);
#endif

		m_clothActors[m_clothActorCount] = actor;
		m_clothActorCount++;

		if (m_state == State::RUNNING)
		{
			RegisterActor(actor);
		}
	}

	void Scene::AddActorInternal(VehicleActor* actor)
	{
#ifdef PHYSICS_DEBUG_MODE
		m_vehicleActorsDebug.push_back(actor);
#endif

		m_vehicleActors[m_vehicleActorCount] = actor;
		m_vehicleActorCount++;

		if (m_state == State::RUNNING)
		{
			RegisterActor(actor);
		}
	}

	void Scene::AddActorInternal(JointActor* actor)
	{
#ifdef PHYSICS_DEBUG_MODE
		m_jointActorsDebug.push_back(actor);
#endif

		m_jointActors[m_jointActorCount] = actor;
		m_jointActorCount++;

		if (m_state == State::RUNNING)
		{
			RegisterActor(actor);
		}
	}

	void Scene::AddActor(Actor* actor)
	{
		switch (actor->GetType())
		{
			case ActorType::Static:
				AddActorInternal((StaticActor*) actor);
				break;

			case ActorType::Dynamic:
				AddActorInternal((DynamicActor*) actor);
				break;

			case ActorType::Cloth:
				AddActorInternal((ClothActor*) actor);
				break;

			case ActorType::Vehicle:
				AddActorInternal((VehicleActor*) actor);
				break;

			case ActorType::Joint:
				AddActorInternal((JointActor*) actor);
				break;

			default:
				std::printf("Unknown Actor Type. Add failed!");
				break;
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

	const Actor** Scene::GetClothActors() const
	{
		return const_cast<const Actor**>(m_clothActors);
	}

	const uint32_t Scene::GetClothActorCount() const
	{
		return m_clothActorCount;
	}

	const Actor** Scene::GetVehicleActors() const
	{
		return const_cast<const Actor**>(m_vehicleActors);
	}
	
	const uint32_t Scene::GetVehicleActorCount() const
	{
		return m_vehicleActorCount;
	}

	const Actor** Scene::GetJointActors() const
	{
		return const_cast<const Actor**>(m_jointActors);
	}

	const uint32_t Scene::GetJointActorCount() const
	{
		return m_jointActorCount;
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
