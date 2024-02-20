#include "Scene.h"
#include "../Utility/CLZ.h"

bool Scene::Init(const SceneConfiguration* configuration)
{
	m_configuration = configuration;

	m_staticActors = new Actor* [m_maxStaticActors];
	m_dynamicActors = new Actor* [m_maxDynamicActors];

	m_tracker = new EventTracker();
	m_tracker->Init(configuration->m_enableDemo);

	return true;
}

bool Scene::PostInit(const physx::PxPhysics* physxObject,
					 const physx::PxCpuDispatcher* dispatcherObject)
{
	physx::PxSceneDesc sceneDesc(physxObject->getTolerancesScale());
	sceneDesc.gravity = m_configuration->m_gravity;
	sceneDesc.filterShader = m_configuration->m_collisionFilter.GetFilter();
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

	physx::PxRigidActor* rigidPxActor = (physx::PxRigidActor*) const_cast<physx::PxActor*>(actor->GetPhysicsActor());

	physx::PxU32 numShapes = rigidPxActor->getNbShapes();
	physx::PxShape** shapes = new physx::PxShape* [numShapes];
	rigidPxActor->getShapes(shapes, numShapes, 0);

	physx::PxFilterData filterData;
	for (physx::PxU32 i = 0; i < numShapes; ++i)
	{
		filterData = shapes[i]->getSimulationFilterData();
		filterData.word0 = (uint32_t) collisionLayer;
		filterData.word1 = m_configuration->m_collisionFilter.GetCollisionMask(collisionIndex);
		shapes[i]->setSimulationFilterData(filterData);
	}

	delete[] shapes;
}

void Scene::Update(float dt)
{
	if (m_state == State::RUNNING)
	{
		// When this call returns, the simulation step has begun in a separate thread
		m_physxScene->simulate(dt);

		// Wait until simulation completes
		m_physxScene->fetchResults(true);
	}
}

void Scene::Release()
{
	m_physxScene->release();
	m_physxScene = nullptr;

	// TODO: Release staticActors and dynamic actors memory?

	m_state = State::UNLOADED;
}

void Scene::LinkEngineScene(void* gameScenePointer)
{
	m_engineScene = gameScenePointer;
}

void Scene::AddActor(StaticActor* actor)
{
	m_staticActors[m_staticActorCount] = actor;
	m_staticActorCount++;

	if (m_state == State::RUNNING)
	{
		printf("Added static actor in runtime! Warning: Very costly operation!");
		RegisterActor(actor);
	}
}

void Scene::RegisterActor(const Actor* actor)
{
	m_physxScene->addActor(*const_cast<physx::PxActor*>(actor->GetPhysicsActor()));
	m_tracker->RegisterAddActorEvent(actor);
}

void Scene::AddActor(DynamicActor* actor)
{
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
