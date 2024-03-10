#include "Actor.h"
#include "../PhysicsEngine.h"
#include "../../Utility/Hashing.h"

namespace PhysicsEngine
{
	Actor::Actor(const uint64_t id, const Type type)
	{
		m_type = type;
		m_actorID = id;
		m_collisionData = CollisionFilter::FilterGroup::Layer_Default;
		m_gameEnginePointerToPhysicsActor = nullptr;
		m_currentPhysxActor = nullptr;
	}

	Actor* Actor::CloneToRender()
	{
		Actor* clone = nullptr;
		CloneActor(clone);

		if (!clone)
		{
			std::printf("Cloning Failed!\n");
			return nullptr;
		}

		// IMPORTANT:
		// We take the game engine pointer, dereference it and replace internal pointer to new physics object
		// We want to do this only for render scenes
		*m_gameEnginePointerToPhysicsActor = clone;

		clone->SetGameEnginePointerToPhysicsActor(m_gameEnginePointerToPhysicsActor);
#ifdef PHYSICS_DEBUG_MODE
		clone->SetGameEnginePointerToPhysicsActorDebug(m_gameEnginePointerToPhysicsActorDebug);
#endif
		clone->SetCollisionLayer((uint32_t) m_collisionData);

		return clone;
	}

	// TODO: Refactor for future scaling
	Actor* Actor::CloneToPhysics()
	{
		Actor* clone = nullptr;
		CloneActor(clone);

		if (!clone)
		{
			std::printf("Cloning Failed!\n");
			return nullptr;
		}

		CloneCallbacks(clone);

		clone->SetGameEnginePointerToPhysicsActor(m_gameEnginePointerToPhysicsActor);
#ifdef PHYSICS_DEBUG_MODE
		clone->SetGameEnginePointerToPhysicsActorDebug(m_gameEnginePointerToPhysicsActorDebug);
#endif
		clone->SetCollisionLayer((uint32_t) m_collisionData);

		return clone;
	}

	void Actor::CloneCallbacks(Actor* clone)
	{
		clone->m_wakeCallbacks = m_wakeCallbacks;
		clone->m_sleepCallbacks = m_sleepCallbacks;
		clone->m_collisionEnterGameCallbacks = m_collisionEnterGameCallbacks;
		clone->m_collisionStayGameCallbacks = m_collisionStayGameCallbacks;
		clone->m_collisionExitGameCallbacks = m_collisionExitGameCallbacks;
		clone->m_triggerEnterGameCallbacks = m_triggerEnterGameCallbacks;
		clone->m_triggerStayGameCallbacks = m_triggerStayGameCallbacks;
		clone->m_triggerExitGameCallbacks = m_triggerExitGameCallbacks;
	}

	void Actor::CloneActor(Actor*& clone)
	{
		ActorFactory* actorFactory = const_cast<ActorFactory*>(PhysicsEngine::Instance()->GetActorFactory());

		if (m_type == Type::Static)
		{
			physx::PxRigidStatic* rigidStaticActor = (physx::PxRigidStatic*) m_currentPhysxActor;

			clone = actorFactory->CreateStaticActor(rigidStaticActor->getGlobalPose());
			physx::PxRigidStatic* cloneStaticActor = (physx::PxRigidStatic*) clone->m_currentPhysxActor;

			cloneStaticActor->setActorFlags(rigidStaticActor->getActorFlags());
			//cloneStaticActor->setBaseFlags(rigidStaticActor->getBaseFlags());

			CloneShapes(rigidStaticActor, clone);
		}

		if (m_type == Type::Dynamic)
		{
			physx::PxRigidDynamic* rigidDynamicActor = (physx::PxRigidDynamic*) m_currentPhysxActor;

			clone = actorFactory->CreateDynamicActor(rigidDynamicActor->getGlobalPose());
			physx::PxRigidDynamic* cloneDynamicActor = (physx::PxRigidDynamic*) clone->m_currentPhysxActor;

			cloneDynamicActor->setActorFlags(rigidDynamicActor->getActorFlags());
			//cloneDynamicActor->setBaseFlags(rigidDynamicActor->getBaseFlags());
			//cloneDynamicActor->setCMassLocalPose(rigidDynamicActor->getCMassLocalPose());
			//cloneDynamicActor->setAngularDamping(rigidDynamicActor->getAngularDamping());
			//cloneDynamicActor->setLinearDamping(rigidDynamicActor->getLinearDamping());
			//cloneDynamicActor->setLinearVelocity(rigidDynamicActor->getLinearVelocity());
			//cloneDynamicActor->setMass(rigidDynamicActor->getMass());
			//cloneDynamicActor->setMassSpaceInertiaTensor(rigidDynamicActor->getMassSpaceInertiaTensor());
			cloneDynamicActor->setRigidBodyFlags(rigidDynamicActor->getRigidBodyFlags());
			//cloneDynamicActor->setRigidDynamicLockFlags(rigidDynamicActor->getRigidDynamicLockFlags());

			CloneShapes(rigidDynamicActor, clone);
		}

		if (m_type == Type::Cloth)
		{
			ClothActor* clothActor = (ClothActor*) this;
			clone = actorFactory->CreateClothActor(clothActor);
		}

		if (!clone)
		{
			std::printf("Actor::CloneActor failed! Unknown m_type!");
			return;
		}
	}

	void Actor::CloneShapes(physx::PxRigidActor* rigidActor, Actor* clone)
	{
		const ShapeCreator* shapeCreator = PhysicsEngine::Instance()->GetShapeCreator();
		const GeometryFactory* geoFactory = PhysicsEngine::Instance()->GetGeometryFactory();

		std::vector<physx::PxShape*> shapes(rigidActor->getNbShapes());
		rigidActor->getShapes(&shapes.front(), shapes.size());

		for (uint32_t i = 0; i < shapes.size(); i++)
		{
			physx::PxGeometry* geometry = geoFactory->Create(shapes[i]->getGeometry());

			// Create either trigger or normal shape
			physx::PxShapeFlags activeShapeFlags = shapes[i]->getFlags();
			if (activeShapeFlags.isSet(physx::PxShapeFlag::eTRIGGER_SHAPE) && !activeShapeFlags.isSet(physx::PxShapeFlag::eSIMULATION_SHAPE))
			{
				shapeCreator->CreateTrigger(clone, geometry, CRC32_STR("Default"));
			}
			else
			{
				shapeCreator->CreateShape(clone, geometry, CRC32_STR("Default"));
			}

			delete geometry;
		}
	}

	void Actor::Release()
	{
		m_actorID = -1;
		m_currentPhysxActor->release();
		m_gameEnginePointerToPhysicsActor = nullptr;
	}

	const uint64_t Actor::GetActorID() const
	{
		return m_actorID;
	}

	const physx::PxActor* Actor::GetCurrentPhysxActor() const
	{
		return m_currentPhysxActor;
	}

	const Actor::Type Actor::GetType() const
	{
		return m_type;
	}

	void Actor::SetCollisionLayer(uint32_t collisionData)
	{
		m_collisionData = (CollisionFilter::FilterGroup) collisionData;
	}

	void Actor::SetGameEnginePointerToPhysicsActor(void** gameEnginePointerToPhysicsActor)
	{
		m_gameEnginePointerToPhysicsActor = gameEnginePointerToPhysicsActor;
	}

	void Actor::SubscribeOnWake(const VoidCallbackEntry& callback)
	{
		m_wakeCallbacks.emplace(callback);
	}

	void Actor::SubscribeOnSleep(const VoidCallbackEntry& callback)
	{
		m_sleepCallbacks.emplace(callback);
	}

	void Actor::SubscribeOnTriggerEnter(const TriggerCallbackEntry& callback)
	{
		m_triggerEnterGameCallbacks.emplace(callback);
	}

	void Actor::SubscribeOnTriggerStay(const TriggerCallbackEntry& callback)
	{
		m_triggerStayGameCallbacks.emplace(callback);
	}

	void Actor::SubscribeOnTriggerExit(const TriggerCallbackEntry& callback)
	{
		m_triggerExitGameCallbacks.emplace(callback);
	}

	void Actor::SubscribeOnCollisionEnter(const CollisionCallbackEntry& callback)
	{
		m_collisionEnterGameCallbacks.emplace(callback);
	}

	void Actor::SubscribeOnCollisionStay(const CollisionCallbackEntry& callback)
	{
		m_collisionStayGameCallbacks.emplace(callback);
	}

	void Actor::SubscribeOnCollisionExit(const CollisionCallbackEntry& callback)
	{
		m_collisionExitGameCallbacks.emplace(callback);
	}

	void Actor::UnsubscribeOnWake(const VoidCallbackEntry& callback)
	{
		m_wakeCallbacks.erase(callback);
	}

	void Actor::UnsubscribeOnSleep(const VoidCallbackEntry& callback)
	{
		m_sleepCallbacks.erase(callback);
	}

	void Actor::UnsubscribeOnTriggerEnter(const TriggerCallbackEntry& callback)
	{
		m_triggerEnterGameCallbacks.erase(callback);
	}

	void Actor::UnsubscribeOnTriggerStay(const TriggerCallbackEntry& callback)
	{
		m_triggerStayGameCallbacks.erase(callback);
	}

	void Actor::UnsubscribeOnTriggerExit(const TriggerCallbackEntry& callback)
	{
		m_triggerExitGameCallbacks.erase(callback);
	}

	void Actor::UnsubscribeOnCollisionEnter(const CollisionCallbackEntry& callback)
	{
		m_collisionEnterGameCallbacks.erase(callback);
	}

	void Actor::UnsubscribeOnCollisionStay(const CollisionCallbackEntry& callback)
	{
		m_collisionStayGameCallbacks.erase(callback);
	}

	void Actor::UnsubscribeOnCollisionExit(const CollisionCallbackEntry& callback)
	{
		m_collisionExitGameCallbacks.erase(callback);
	}

#ifdef PHYSICS_DEBUG_MODE
	void Actor::SetGameEnginePointerToPhysicsActorDebug(Actor** gameEnginePointerToPhysicsActorDebug)
	{
		m_gameEnginePointerToPhysicsActorDebug = gameEnginePointerToPhysicsActorDebug;
	}
#endif

	CollisionFilter::FilterGroup Actor::GetCollisionLayer() const
	{
		return m_collisionData;
	}

	void Actor::OnWake()
	{
		for (auto& entry : m_wakeCallbacks)
		{
			entry.m_callback();
		}
	}

	void Actor::OnSleep()
	{
		for (auto& entry : m_sleepCallbacks)
		{
			entry.m_callback();
		}
	}

	void Actor::OnTriggerEnter(physx::PxShape* triggerShape, Actor* otherActor, physx::PxShape* otherShape)
	{
		for (auto& entry : m_triggerEnterGameCallbacks)
		{
			entry.m_callback(nullptr, nullptr, nullptr, nullptr);
		}
	}

	void Actor::OnTriggerStay(physx::PxShape* triggerShape, Actor* otherActor, physx::PxShape* otherShape)
	{
		for (auto& entry : m_triggerStayGameCallbacks)
		{
			entry.m_callback(nullptr, nullptr, nullptr, nullptr);
		}
	}

	void Actor::OnTriggerExit(physx::PxShape* triggerShape, Actor* otherActor, physx::PxShape* otherShape)
	{
		for (auto& entry : m_triggerExitGameCallbacks)
		{
			entry.m_callback(nullptr, nullptr, nullptr, nullptr);
		}
	}

	void Actor::OnCollisionEnter(Actor* otherActor, std::vector<CustomSimulationEventCallback::ContactInfo*> contactsData)
	{
		for (auto& entry : m_collisionEnterGameCallbacks)
		{
			entry.m_callback(nullptr, nullptr, nullptr, nullptr);
		}
	}

	void Actor::OnCollisionStay(Actor* otherActor, std::vector<CustomSimulationEventCallback::ContactInfo*> contactsData)
	{
		for (auto& entry : m_collisionStayGameCallbacks)
		{
			entry.m_callback(nullptr, nullptr, nullptr, nullptr);
		}
	}

	void Actor::OnCollisionExit(Actor* otherActor, std::vector<CustomSimulationEventCallback::ContactInfo*> contactsData)
	{
		for (auto& entry : m_collisionExitGameCallbacks)
		{
			entry.m_callback(nullptr, nullptr, nullptr, nullptr);
		}
	}
}
