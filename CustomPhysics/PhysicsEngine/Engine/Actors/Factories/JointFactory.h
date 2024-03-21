#pragma once

#ifndef JointFactory_H
#define JointFactory_H

#include <atomic>
#include <PxPhysicsAPI.h>
#include "../JointActor.h"

namespace PhysicsEngine
{
	class JointFactory
	{
		friend class ActorFactory;

	private:
		static uint64_t GenerateId();
		static std::atomic<uint64_t> s_lastId;

		physx::PxPhysics* m_physicsRef;

	private:
		bool Init(physx::PxPhysics* physics);
		
		JointActor* CreateFixedJoint(const uint64_t actorId, physx::PxRigidActor* actor0, const physx::PxTransform& localFrame0, physx::PxRigidActor* actor1, const physx::PxTransform& localFrame1);
		JointActor* CreateDistanceJoint(const uint64_t actorId, physx::PxRigidActor* actor0, const physx::PxTransform& localFrame0, physx::PxRigidActor* actor1, const physx::PxTransform& localFrame1, physx::PxJointLimitParameters* limitParams);
		JointActor* CreateSphericalJoint(const uint64_t actorId, physx::PxRigidActor* actor0, const physx::PxTransform& localFrame0, physx::PxRigidActor* actor1, const physx::PxTransform& localFrame1, physx::PxJointLimitCone* coneLimitParams);
		JointActor* CreateRevoluteJoint(const uint64_t actorId, physx::PxRigidActor* actor0, const physx::PxTransform& localFrame0, physx::PxRigidActor* actor1, const physx::PxTransform& localFrame1, physx::PxJointAngularLimitPair* angularLimitParams);
		JointActor* CreatePrismaticJoint(const uint64_t actorId, physx::PxRigidActor* actor0, const physx::PxTransform& localFrame0, physx::PxRigidActor* actor1, const physx::PxTransform& localFrame1, physx::PxJointLinearLimitPair* linearLimitParams);
		JointActor* CreateGearJoint(const uint64_t actorId, physx::PxRigidActor* actor0, const physx::PxTransform& localFrame0, physx::PxRigidActor* actor1, const physx::PxTransform& localFrame1);
		JointActor* CreateRackAndPinionJoint(const uint64_t actorId, physx::PxRigidActor* actor0, const physx::PxTransform& localFrame0, physx::PxRigidActor* actor1, const physx::PxTransform& localFrame1);
		JointActor* CreateD6Joint(const uint64_t actorId, physx::PxRigidActor* actor0, const physx::PxTransform& localFrame0, physx::PxRigidActor* actor1, const physx::PxTransform& localFrame1, physx::PxJointLinearLimit* linearLimitParams, physx::PxJointAngularLimitPair* angularLimitParams, physx::PxJointLimitCone* coneLimitParams);
	};
}

#endif