#pragma once

#ifndef Scene_H
#define Scene_H

#include "PxPhysicsAPI.h"
#include <PxScene.h>
#include "../GlobalDefine.h"
#include "Physics.h"
#include "Dispatcher.h"
#include "CollisionFilter.h"
#include "Actors/StaticActor.h"
#include "Actors/DynamicActor.h"
#include "EventTracker/EventTracker.h"
#include <common/PxRenderBuffer.h>

class alignas(8) Scene
{
	friend class SceneManager;

private:
	enum class State
	{
		UNLOADED,
		RUNNING,
		PAUSED,
	};

public:
	struct alignas(8) SceneConfiguration
	{
		CollisionFilter m_collisionFilter;
		physx::PxVec3 m_gravity;
		bool m_enableDemo;
		bool _padding[3];
	};

	const uint32_t m_maxStaticActors = 128;
	const uint32_t m_maxDynamicActors = 255;

private:
	physx::PxScene* m_physxScene;
	const SceneConfiguration* m_configuration;

#ifdef DEBUG_MODE
	void* m_engineScene;
#endif

	EventTracker* m_tracker;
	State m_state;

	Actor** m_staticActors;
	Actor** m_dynamicActors;
	uint32_t m_staticActorCount;
	uint32_t m_dynamicActorCount;

	bool Init(const SceneConfiguration* configuration);
	bool PostInit(const physx::PxPhysics* physxObject,
			  const physx::PxCpuDispatcher* dispatcherObject);
	void SetupActorFilter(const Actor* actor);
	void Update(float dt);
	void Release();

	void RegisterActor(const Actor* actor);

public:
#ifdef DEBUG_MODE
	void LinkEngineScene(void* gameScenePointer);
#endif

	void AddActor(StaticActor* actor);
	void AddActor(DynamicActor* actor);

	const Actor** GetStaticActors() const;
	const uint32_t GetStaticActorCount() const;
	const Actor** GetDynamicActors() const;
	const uint32_t GetDynamicActorCount() const;

	const physx::PxScene* GetPhysxScene() const;
	const physx::PxRenderBuffer& GetRenderBuffer() const;
};

#endif