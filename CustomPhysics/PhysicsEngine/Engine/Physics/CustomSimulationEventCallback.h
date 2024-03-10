#pragma once

#ifndef CustomSimulationEventCallback_H
#define CustomSimulationEventCallback_H

#include <PxSimulationEventCallback.h>

namespace PhysicsEngine
{
	class CustomSimulationEventCallback : public physx::PxSimulationEventCallback
	{
	public:
		struct ContactInfo
		{
			physx::PxVec3 m_contactPosition;
			physx::PxVec3 m_contactImpulse;
			physx::PxVec3 m_contactNormal;

			ContactInfo(const physx::PxVec3& position, const physx::PxVec3& impulse, const physx::PxVec3& normal) :
				m_contactPosition(position), m_contactImpulse(impulse), m_contactNormal(normal)
			{
			}
		};

		CustomSimulationEventCallback() = default;

		void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) override;

		void onWake(physx::PxActor** actors, physx::PxU32 count) override;
		void onSleep(physx::PxActor** actors, physx::PxU32 count) override;

		void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs) override;
		void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) override;

		// Provides early access to the new pose of moving rigid bodies (should be a lighweight task as may block fetchresult call)
		void onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count) override;
	};
}

#endif