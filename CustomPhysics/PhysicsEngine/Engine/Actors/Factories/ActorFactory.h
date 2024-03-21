#pragma once

#ifndef ActorFactory_H
#define ActorFactory_H

#include <atomic>
#include "../Actor.h"
#include "../ClothActor.h"
#include "../../Types/VehicleData.h"
#include "../../../Utility/VehicleCreator.h"
#include "JointFactory.h"
#include "../../Types/JointType.h"
#include "../../Types/JointActorConfig.h"

namespace PhysicsEngine
{
	class ActorFactory
	{
	private:
		static uint64_t GenerateId();
		static std::atomic<uint64_t> s_lastId;

		physx::PxPhysics* m_physics;

		JointFactory* m_jointFactory;
		VehicleCreator* m_vehicleCreator;

	public:
		bool Init(const physx::PxPhysics* physics);

		Actor* CloneClothActor(const ClothActor* objCopyDataFrom);

		// TODO: API Exposed
		Actor* CreateStaticActor(const physx::PxTransform& transform);
		Actor* CreateDynamicActor(const physx::PxTransform& transform);
		Actor* CreateKinematicActor(const physx::PxTransform& transform);
		Actor* CreateClothActor(const physx::PxTransform& transform, const physx::PxVec2& size = physx::PxVec2(1.f, 1.f), const physx::PxVec2& scale = physx::PxVec2(1.f, 1.f));
		Actor* CreateJointActor(const JointActorConfig& jointConfig);
		Actor* CreateVehicleActor(const physx::PxTransform& transform, const VehicleData* configData);

	};
}

#endif