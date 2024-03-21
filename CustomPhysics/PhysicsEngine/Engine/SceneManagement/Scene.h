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
#include "../Actors/VehicleActor.h"
#include "../Actors/JointActor.h"

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
			VEHICLE = 1 << 3,
			JOINT = 1 << 4,
			RUNTIME_UPDATE = DYNAMIC | CLOTH | VEHICLE | JOINT,
			ALL = STATIC | DYNAMIC | CLOTH | VEHICLE | JOINT,
		};

		static const uint32_t k_maxStaticActors = 32;
		static const uint32_t k_maxDynamicActors = 32;
		static const uint32_t k_maxClothActors = 32;
		static const uint32_t k_maxVehicleActors = 8;
		static const uint32_t k_maxJointActors = 64;

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
		Actor** m_vehicleActors;
		Actor** m_jointActors;

		uint32_t m_staticActorCount;
		uint32_t m_dynamicActorCount;
		uint32_t m_clothActorCount;
		uint32_t m_vehicleActorCount;
		uint32_t m_jointActorCount;

		physx::PxVehicleWheels** m_vehicles;
		physx::PxRaycastQueryResult* m_sqResults;

#ifdef PHYSICS_DEBUG_MODE
		std::vector<Actor*> m_staticActorsDebug;
		std::vector<Actor*> m_dynamicActorsDebug;
		std::vector<Actor*> m_clothActorsDebug;
		std::vector<Actor*> m_vehicleActorsDebug;
		std::vector<Actor*> m_jointActorsDebug;
#endif

		bool Init(const SceneConfiguration* configuration);
		bool PostInit(const physx::PxPhysics* physxObject,
					  const Dispatcher* dispatcherObject);
		void SetupActorFilter(const Actor* actor);
		void Prepare();
		void Update(float dt);
		void Release();

		void RegisterActor(const Actor* actor);
		void Sync(Scene* sourceScene, SyncState syncState);

		void StaticSync(Scene* sourceScene);
		void DynamicSync(Scene* sourceScene);
		void ClothSync(Scene* sourceScene);
		void VehicleSync(Scene* sourceScene);
		void JointSync(Scene* sourceScene);

		Scene() = delete;
		Scene(uint32_t id);

		void AddActorInternal(StaticActor* actor);
		void AddActorInternal(DynamicActor* actor);
		void AddActorInternal(ClothActor* actor);
		void AddActorInternal(VehicleActor* actor);
		void AddActorInternal(JointActor* actor);

	public:
		// TODO: API Expose
		void Lock();
		void Unlock();
		void LinkEngineScene(void* gameScenePointer);

		void AddActor(Actor* actor);

		const Actor** GetStaticActors() const;
		const uint32_t GetStaticActorCount() const;

		const Actor** GetDynamicActors() const;
		const uint32_t GetDynamicActorCount() const;

		const Actor** GetClothActors() const;
		const uint32_t GetClothActorCount() const;

		const Actor** GetVehicleActors() const;
		const uint32_t GetVehicleActorCount() const;

		const Actor** GetJointActors() const;
		const uint32_t GetJointActorCount() const;

		const physx::PxScene* GetPhysxScene() const;
		const physx::PxRenderBuffer& GetRenderBuffer() const;
	};
}


#endif