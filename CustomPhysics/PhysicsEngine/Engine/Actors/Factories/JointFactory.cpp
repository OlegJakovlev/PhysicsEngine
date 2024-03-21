#include "JointFactory.h"

namespace PhysicsEngine
{
    std::atomic<uint64_t> JointFactory::s_lastId = 0;

    uint64_t JointFactory::GenerateId()
    {
        return s_lastId++;
    }

    bool JointFactory::Init(physx::PxPhysics* physics)
    {
        m_physicsRef = physics;
        return true;
    }

    JointActor* JointFactory::CreateFixedJoint(const uint64_t actorId, physx::PxRigidActor* actor0, const physx::PxTransform& localFrame0, physx::PxRigidActor* actor1, const physx::PxTransform& localFrame1)
    {
        JointActor* actor = new JointActor(actorId, GenerateId());
        auto physxJoint = physx::PxFixedJointCreate(*m_physicsRef, actor0, localFrame0, actor1, localFrame1);
        actor->SetPhysxJoint(physxJoint);
        return actor;
    }

    JointActor* JointFactory::CreateDistanceJoint(const uint64_t actorId, physx::PxRigidActor* actor0, const physx::PxTransform& localFrame0, physx::PxRigidActor* actor1, const physx::PxTransform& localFrame1, physx::PxJointLimitParameters* limitParams)
    {
        JointActor* actor = new JointActor(actorId, GenerateId());
        auto physxJoint = physx::PxDistanceJointCreate(*m_physicsRef, actor0, localFrame0, actor1, localFrame1);
        
        if (limitParams)
        {
            physxJoint->setDamping(limitParams->damping);
            physxJoint->setStiffness(limitParams->stiffness);
            physxJoint->setTolerance(limitParams->contactDistance);
        }
        
        actor->SetPhysxJoint(physxJoint);
        return actor;
    }

    JointActor* JointFactory::CreateSphericalJoint(const uint64_t actorId, physx::PxRigidActor* actor0, const physx::PxTransform& localFrame0, physx::PxRigidActor* actor1, const physx::PxTransform& localFrame1, physx::PxJointLimitCone* coneLimitParams)
    {
        JointActor* actor = new JointActor(actorId, GenerateId());
        auto physxJoint = physx::PxSphericalJointCreate(*m_physicsRef, actor0, localFrame0, actor1, localFrame1);

        if (coneLimitParams)
        {
            physxJoint->setLimitCone(*coneLimitParams);
        }

        actor->SetPhysxJoint(physxJoint);
        return actor;
    }

    JointActor* JointFactory::CreateRevoluteJoint(const uint64_t actorId, physx::PxRigidActor* actor0, const physx::PxTransform& localFrame0, physx::PxRigidActor* actor1, const physx::PxTransform& localFrame1, physx::PxJointAngularLimitPair* angularLimitParams)
    {
        JointActor* actor = new JointActor(actorId, GenerateId());
        auto physxJoint = physx::PxRevoluteJointCreate(*m_physicsRef, actor0, localFrame0, actor1, localFrame1);

        if (angularLimitParams)
        {
            physxJoint->setLimit(*angularLimitParams);
        }
        
        actor->SetPhysxJoint(physxJoint);
        return actor;
    }

    JointActor* JointFactory::CreatePrismaticJoint(const uint64_t actorId, physx::PxRigidActor* actor0, const physx::PxTransform& localFrame0, physx::PxRigidActor* actor1, const physx::PxTransform& localFrame1, physx::PxJointLinearLimitPair* linearLimitParams)
    {
        JointActor* actor = new JointActor(actorId, GenerateId());
        auto physxJoint = physx::PxPrismaticJointCreate(*m_physicsRef, actor0, localFrame0, actor1, localFrame1);

        if (linearLimitParams)
        {
            physxJoint->setLimit(*linearLimitParams);
        }

        actor->SetPhysxJoint(physxJoint);
        return actor;
    }

    JointActor* JointFactory::CreateGearJoint(const uint64_t actorId, physx::PxRigidActor* actor0, const physx::PxTransform& localFrame0, physx::PxRigidActor* actor1, const physx::PxTransform& localFrame1)
    {
#if PX_PHYSICS_VERSION < 0x5010000
        std::printf("Gear Joint is not supported! Please update the SDK!");
        return nullptr;
#else
        JointActor* actor = new JointActor(actorId, GenerateId());
        actor->SetPhysxJoint(physx::PxGearJointCreate(*m_physicsRef, actor0, localFrame0, actor1, localFrame1));
        return actor;
#endif
    }

    JointActor* JointFactory::CreateRackAndPinionJoint(const uint64_t actorId, physx::PxRigidActor* actor0, const physx::PxTransform& localFrame0, physx::PxRigidActor* actor1, const physx::PxTransform& localFrame1)
    {
#if PX_PHYSICS_VERSION < 0x5010000
        std::printf("CreateRackAndPinionJoint Joint is not supported! Please update the SDK!");
        return nullptr;
#else
        JointActor* actor = new JointActor(actorId, GenerateId());
        actor->SetPhysxJoint(physx::PxRackAndPinionJointCreate(*m_physicsRef, actor0, localFrame0, actor1, localFrame1));
        return actor;
#endif
    }

    JointActor* JointFactory::CreateD6Joint(const uint64_t actorId, physx::PxRigidActor* actor0, const physx::PxTransform& localFrame0, physx::PxRigidActor* actor1, const physx::PxTransform& localFrame1, physx::PxJointLinearLimit* linearLimitParams, physx::PxJointAngularLimitPair* angularLimitParams, physx::PxJointLimitCone* coneLimitParams)
    {
        JointActor* actor = new JointActor(actorId, GenerateId());
        auto physxJoint = physx::PxD6JointCreate(*m_physicsRef, actor0, localFrame0, actor1, localFrame1);

        if (linearLimitParams)
        {
            physxJoint->setLinearLimit(*linearLimitParams);
        }

        if (coneLimitParams)
        {
            physxJoint->setSwingLimit(*coneLimitParams);
        }
        
        if (angularLimitParams)
        {
            physxJoint->setTwistLimit(*angularLimitParams);
        }
        
        actor->SetPhysxJoint(physxJoint);
        return actor;
    }
}

