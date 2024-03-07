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

		clone->m_gameEnginePointerToPhysicsActor = m_gameEnginePointerToPhysicsActor;
		clone->m_collisionData = m_collisionData;

		if (m_currentPhysxActor->is<physx::PxRigidActor>())
		{
			physx::PxRigidActor* rigidActor = (physx::PxRigidActor*)m_currentPhysxActor;
			physx::PxRigidActor* cloneRigidActor = (physx::PxRigidActor*)clone->m_currentPhysxActor;

			CloneShapes(rigidActor, clone);
		}

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

		clone->m_gameEnginePointerToPhysicsActor = m_gameEnginePointerToPhysicsActor;
		clone->m_collisionData = m_collisionData;

		if (m_currentPhysxActor->is<physx::PxRigidActor>())
		{
			physx::PxRigidActor* rigidActor = (physx::PxRigidActor*) m_currentPhysxActor;
			physx::PxRigidActor* cloneRigidActor = (physx::PxRigidActor*)clone->m_currentPhysxActor;

			CloneShapes(rigidActor, clone);
		}

		return clone;
	}

	void Actor::CloneActor(Actor*& clone)
	{
		ActorFactory* actorFactory = const_cast<ActorFactory*>(PhysicsEngine::Instance()->GetActorFactory());

		if (m_type == Type::Static)
		{
			physx::PxRigidStatic* rigidStaticActor = (physx::PxRigidStatic*) m_currentPhysxActor;

			clone = actorFactory->CreateStaticActor(rigidStaticActor->getGlobalPose());
			physx::PxRigidStatic* cloneStaticActor = (physx::PxRigidStatic*) clone->m_currentPhysxActor;

			//cloneStaticActor->setActorFlags(rigidStaticActor->getActorFlags());
			//cloneStaticActor->setBaseFlags(rigidStaticActor->getBaseFlags());
			return;
		}

		if (m_type == Type::Dynamic)
		{
			physx::PxRigidDynamic* rigidDynamicActor = (physx::PxRigidDynamic*) m_currentPhysxActor;

			clone = actorFactory->CreateDynamicActor(rigidDynamicActor->getGlobalPose());
			physx::PxRigidDynamic* cloneDynamicActor = (physx::PxRigidDynamic*) clone->m_currentPhysxActor;

			//cloneDynamicActor->setActorFlags(rigidDynamicActor->getActorFlags());
			//cloneDynamicActor->setBaseFlags(rigidDynamicActor->getBaseFlags());
			//cloneDynamicActor->setCMassLocalPose(rigidDynamicActor->getCMassLocalPose());
			//cloneDynamicActor->setAngularDamping(rigidDynamicActor->getAngularDamping());
			//cloneDynamicActor->setLinearDamping(rigidDynamicActor->getLinearDamping());
			//cloneDynamicActor->setLinearVelocity(rigidDynamicActor->getLinearVelocity());
			//cloneDynamicActor->setMass(rigidDynamicActor->getMass());
			//cloneDynamicActor->setMassSpaceInertiaTensor(rigidDynamicActor->getMassSpaceInertiaTensor());
			cloneDynamicActor->setRigidBodyFlags(rigidDynamicActor->getRigidBodyFlags());
			//cloneDynamicActor->setRigidDynamicLockFlags(rigidDynamicActor->getRigidDynamicLockFlags());
			return;
		}

		if (m_type == Type::Cloth)
		{
			physx::PxCloth* clothActor = (physx::PxCloth*) m_currentPhysxActor;

			physx::PxU32* quadContainer = nullptr;
			physx::PxU32 quadAmount;

			clone = actorFactory->CreateClothActor(clothActor->getGlobalPose());
			
			if (quadContainer)
			{
				delete quadContainer;
			}

			return;
		}

		std::printf("Actor::CloneActor failed! Unknown m_type!");
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
}
