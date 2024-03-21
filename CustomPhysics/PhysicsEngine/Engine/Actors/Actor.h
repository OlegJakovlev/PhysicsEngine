#pragma once

#ifndef Actor_H
#define Actor_H

#include "../../GlobalDefine.h"
#include <cstdint>
#include <PxPhysicsAPI.h>
#include <unordered_set>
#include "../Physics/CollisionFilter.h"
#include "../Components/TriggerCallbackEntry.h"
#include "../Components/CollisionCallbackEntry.h"
#include "../Components/VoidCallbackEntry.h"
#include "../Physics/CustomSimulationEventCallback.h"
#include "../Types/ContactInfo.h"
#include "../Types/ActorType.h"

namespace PhysicsEngine
{
	class Actor
	{
	private:
		void CloneActor(Actor*& clone);
		void CloneShapes(physx::PxRigidActor* rigidActor, Actor* clone);
		void CloneCallbacks(Actor* clone);

	protected:
		uint64_t m_actorID;
		physx::PxActor* m_currentPhysxActor;
		void** m_gameEnginePointerToPhysicsActor;

#ifdef PHYSICS_DEBUG_MODE
		Actor** m_gameEnginePointerToPhysicsActorDebug;
#endif

		ActorType m_type;
		FilterGroup m_collisionLayer;
		FilterNumericGroup m_collisionLayerIndex;

		std::unordered_set<VoidCallbackEntry> m_wakeCallbacks;
		std::unordered_set<VoidCallbackEntry> m_sleepCallbacks;

		std::unordered_set<TriggerCallbackEntry> m_triggerEnterGameCallbacks;
		std::unordered_set<TriggerCallbackEntry> m_triggerStayGameCallbacks;
		std::unordered_set<TriggerCallbackEntry> m_triggerExitGameCallbacks;

		std::unordered_set<CollisionCallbackEntry> m_collisionEnterGameCallbacks;
		std::unordered_set<CollisionCallbackEntry> m_collisionStayGameCallbacks;
		std::unordered_set<CollisionCallbackEntry> m_collisionExitGameCallbacks;

		Actor(const uint64_t id, const ActorType type);

	public:
		Actor* CloneToRender();
		Actor* CloneToPhysics();

		virtual void Release();

		const uint64_t GetActorID() const;
		const physx::PxActor* GetCurrentPhysxActor() const;
		const ActorType GetType() const;
		FilterGroup GetCollisionLayer() const;
		FilterNumericGroup GetCollisionLayerIndex() const;

		void OnWake();
		void OnSleep();

		void OnTriggerEnter(physx::PxShape* triggerShape, Actor* otherActor, physx::PxShape* otherShape);
		void OnTriggerStay(physx::PxShape* triggerShape, Actor* otherActor, physx::PxShape* otherShape);
		void OnTriggerExit(physx::PxShape* triggerShape, Actor* otherActor, physx::PxShape* otherShape);

		void OnCollisionEnter(Actor* otherActor, std::vector<ContactInfo*> contactsData);
		void OnCollisionStay(Actor* otherActor, std::vector<ContactInfo*> contactsData);
		void OnCollisionExit(Actor* otherActor, std::vector<ContactInfo*> contactsData);

		// TODO: API Expose
		void SetCollisionLayer(uint32_t collisionData);
		void SetGameEnginePointerToPhysicsActor(void** gameEnginePointerToPhysicsActor);

		void SubscribeOnWake(const VoidCallbackEntry& callback);
		void SubscribeOnSleep(const VoidCallbackEntry& callback);
		void SubscribeOnTriggerEnter(const TriggerCallbackEntry& callback);
		void SubscribeOnTriggerStay(const TriggerCallbackEntry& callback);
		void SubscribeOnTriggerExit(const TriggerCallbackEntry& callback);
		void SubscribeOnCollisionEnter(const CollisionCallbackEntry& callback);
		void SubscribeOnCollisionStay(const CollisionCallbackEntry& callback);
		void SubscribeOnCollisionExit(const CollisionCallbackEntry& callback);

		void UnsubscribeOnWake(const VoidCallbackEntry& callback);
		void UnsubscribeOnSleep(const VoidCallbackEntry& callback);
		void UnsubscribeOnTriggerEnter(const TriggerCallbackEntry& callback);
		void UnsubscribeOnTriggerStay(const TriggerCallbackEntry& callback);
		void UnsubscribeOnTriggerExit(const TriggerCallbackEntry& callback);
		void UnsubscribeOnCollisionEnter(const CollisionCallbackEntry& callback);
		void UnsubscribeOnCollisionStay(const CollisionCallbackEntry& callback);
		void UnsubscribeOnCollisionExit(const CollisionCallbackEntry& callback);

#ifdef PHYSICS_DEBUG_MODE
		void SetGameEnginePointerToPhysicsActorDebug(Actor** gameEnginePointerToPhysicsActorDebug);
#endif
	};
}

#endif