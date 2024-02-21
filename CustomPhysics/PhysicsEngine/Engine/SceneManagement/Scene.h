#pragma once

#ifndef Scene_H
#define Scene_H

#include "PxPhysicsAPI.h"
#include <PxScene.h>
#include "../../GlobalDefine.h"
#include "../Services/Physics.h"
#include "../Services/Dispatcher.h"
#include "../Physics/CollisionFilter.h"
#include "../Actors/StaticActor.h"
#include "../Actors/DynamicActor.h"
#include "../EventTracker/EventTracker.h"
#include <common/PxRenderBuffer.h>
#include <vector>
#include <mutex>

namespace PhysicsEngine
{
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

		static const uint32_t k_maxStaticActors = 128;
		static const uint32_t k_maxDynamicActors = 255;

	public:
		struct alignas(8) SceneConfiguration
		{
			CollisionFilter* m_collisionFilter;
			physx::PxVec3 m_gravity;
			bool m_enableDemo;
			bool _padding[3];

			SceneConfiguration();
			SceneConfiguration(const SceneConfiguration& sceneConfig);
			~SceneConfiguration();
		};

	private:
		physx::PxScene* m_physxScene;
		const SceneConfiguration* m_configuration;
		void* m_engineScene;

		EventTracker* m_tracker;
		State m_state;
		std::mutex m_mutex;

		Actor** m_staticActors;
		Actor** m_dynamicActors;

		uint32_t m_staticActorCount;
		uint32_t m_dynamicActorCount;

#ifdef PHYSICS_DEBUG_MODE
		std::vector<Actor*> m_staticActorsDebug;
		std::vector<Actor*> m_dynamicActorsDebug;
#endif

		bool Init(const SceneConfiguration* configuration);
		bool PostInit(const physx::PxPhysics* physxObject,
					  const physx::PxCpuDispatcher* dispatcherObject);
		void SetupActorFilter(const Actor* actor);
		void Update(float dt);
		void Release();

		void RegisterActor(const Actor* actor);
		void StaticSync(Scene* sourceScene);
		void DynamicSync(Scene* sourceScene);

		Scene();
	public:
		// TODO: API Expose
		void Lock();
		void Unlock();
		void LinkEngineScene(void* gameScenePointer);

		void AddActor(StaticActor* actor);
		void AddActor(DynamicActor* actor);

		const Actor** GetStaticActors() const;
		const uint32_t GetStaticActorCount() const;
		const Actor** GetDynamicActors() const;
		const uint32_t GetDynamicActorCount() const;

		const physx::PxScene* GetPhysxScene() const;
		const physx::PxRenderBuffer& GetRenderBuffer() const;
	};
}


#endif