#pragma once

#ifndef CustomSimulationEventCallback_H
#define CustomSimulationEventCallback_H

#include <PxSimulationEventCallback.h>

namespace PhysicsEngine
{
	class CustomSimulationEventCallback : public physx::PxSimulationEventCallback
	{
	public:
		CustomSimulationEventCallback() = default;

		void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) override;

		void onWake(physx::PxActor** actors, physx::PxU32 count) override;
		void onSleep(physx::PxActor** actors, physx::PxU32 count) override;

		void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs) override;
		void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) override;

		// Provides early access to the new pose of moving rigid bodies (should be a light-weight task as may block fetch result call)
		void onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count) override;
	};
}

#endif