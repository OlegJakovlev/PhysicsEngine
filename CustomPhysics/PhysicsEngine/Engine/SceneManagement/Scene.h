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
#include "../Actors/ClothActor.h"

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

		enum SyncState
		{
			STATIC = 1 << 0,
			DYNAMIC = 1 << 1,
			CLOTH = 1 << 2,
			RUNTIME_UPDATE = DYNAMIC | CLOTH,
			ALL = STATIC | DYNAMIC | CLOTH,
		};

		static const uint32_t k_maxStaticActors = 128;
		static const uint32_t k_maxDynamicActors = 255;
		static const uint32_t k_maxClothActors = 64;

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
		const uint32_t m_id;

		std::mutex m_mutex;

		Actor** m_staticActors;
		Actor** m_dynamicActors;
		Actor** m_clothActors;

		uint32_t m_staticActorCount;
		uint32_t m_dynamicActorCount;
		uint32_t m_clothActorCount;

#ifdef PHYSICS_DEBUG_MODE
		std::vector<Actor*> m_staticActorsDebug;
		std::vector<Actor*> m_dynamicActorsDebug;
		std::vector<Actor*> m_clothActorsDebug;
#endif

		bool Init(const SceneConfiguration* configuration);
		bool PostInit(const physx::PxPhysics* physxObject,
					  const Dispatcher* dispatcherObject);
		void SetupActorFilter(const Actor* actor);
		void Update(float dt);
		void Release();

		void RegisterActor(const Actor* actor);
		void Sync(Scene* sourceScene, SyncState syncState);

		void StaticSync(Scene* sourceScene);
		void DynamicSync(Scene* sourceScene);
		void ClothSync(Scene* sourceScene);

		Scene() = delete;
		Scene(uint32_t id);

	public:
		// TODO: API Expose
		void Lock();
		void Unlock();
		void LinkEngineScene(void* gameScenePointer);

		// TODO: Replace with void* with casting and type checking inside
		void AddActor(StaticActor* actor);
		void AddActor(DynamicActor* actor);
		void AddActor(ClothActor* actor);

		const Actor** GetStaticActors() const;
		const uint32_t GetStaticActorCount() const;
		const Actor** GetDynamicActors() const;
		const uint32_t GetDynamicActorCount() const;
		const Actor** GetClothActors() const;
		const uint32_t GetClothActorCount() const;

		const physx::PxScene* GetPhysxScene() const;
		const physx::PxRenderBuffer& GetRenderBuffer() const;
	};
}


#endif