#pragma once

#ifndef JointActor_H
#define JointActor_H

#include "PxPhysicsAPI.h"
#include "Actor.h"
#include "../Types/JointActorConfig.h"

namespace PhysicsEngine
{
	class JointActor : public Actor
	{
		friend class JointFactory;
		
	private:
		physx::PxJoint* m_physxJoint;
		JointActorConfig* m_actorConfig;
		uint64_t m_jointId;

	private:
		JointActor(uint64_t actorId, uint64_t jointId);
		void SetPhysxJoint(physx::PxJoint* joint);

	public:
		const physx::PxJoint* JointActor::GetPhysxJoint() const;
		const JointActorConfig* GetJointActorConfig() const;
	};
}

#endif