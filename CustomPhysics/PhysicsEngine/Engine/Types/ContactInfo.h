#pragma once

#ifndef ContactInfo_H
#define ContactInfo_H

#include "PxPhysicsAPI.h"

struct ContactInfo
{
	physx::PxVec3 m_contactPosition;
	physx::PxVec3 m_contactImpulse;
	physx::PxVec3 m_contactNormal;

	ContactInfo(const physx::PxVec3& position, const physx::PxVec3& impulse, const physx::PxVec3& normal) :
		m_contactPosition(position),
		m_contactImpulse(impulse),
		m_contactNormal(normal)
	{
	}
};

#endif