#pragma once

#ifndef JointActorConfig_H
#define JointActorConfig_H


#include "PxPhysicsAPI.h"
#include "JointType.h"

namespace PhysicsEngine
{
	struct JointActorConfig
	{
		physx::PxRigidActor* actor0;
		const physx::PxTransform& localFrame0;
		physx::PxRigidActor* actor1;
		const physx::PxTransform& localFrame1;
		
		JointType type;

		// Breakable config
		float forceThreshold = FLT_MAX;
		float torqueThreshold = FLT_MAX;

		// Limits
		// TODO: Any refactoring pattern?
		physx::PxJointLimitParameters* limits;
		physx::PxJointAngularLimitPair* angularLimitPair;
		physx::PxJointLinearLimit* linearLimit;
		physx::PxJointLinearLimitPair* linearLimitPair;
		physx::PxJointLimitCone* limitCone;
	};
}

#endif