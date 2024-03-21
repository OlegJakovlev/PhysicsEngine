#include "JointActor.h"

namespace PhysicsEngine
{
	JointActor::JointActor(uint64_t actorId, uint64_t jointId) : Actor(actorId, Joint)
	{
		m_jointId = jointId;
		m_physxJoint = nullptr;
	}

	void JointActor::SetPhysxJoint(physx::PxJoint* joint)
	{
		m_physxJoint = joint;
	}

	const physx::PxJoint* JointActor::GetPhysxJoint() const
	{
		return m_physxJoint;
	}

	const JointActorConfig* JointActor::GetJointActorConfig() const
	{
		return m_actorConfig;
	}
}