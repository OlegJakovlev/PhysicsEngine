#include "Cooking.h"

namespace PhysicsEngine
{
	bool Cooking::Init(const physx::PxFoundation* foundation, const physx::PxPhysics* physics)
	{
		physx::PxFoundation* foundationObject = const_cast<physx::PxFoundation*>(foundation);

		m_cooking = PxCreateCooking(PX_PHYSICS_VERSION, *foundationObject, physx::PxCookingParams(physics->getTolerancesScale()));

		return true;
	}

	void Cooking::Release()
	{
		m_cooking->release();
		m_cooking = nullptr;
	}

	const physx::PxCooking* Cooking::GetCooking() const
	{
		return m_cooking;
	}
}

