#include "ActorFactory.h"
#include "StaticActor.h"
#include "DynamicActor.h"
#include "ClothActor.h"

namespace PhysicsEngine
{
	std::atomic<uint64_t> ActorFactory::s_lastId = 0;

	uint64_t ActorFactory::GenerateId()
	{
		return ++s_lastId;
	}

	bool ActorFactory::Init(const physx::PxPhysics* physics)
	{
		m_physics = const_cast<physx::PxPhysics*>(physics);
		return true;
	}

	Actor* ActorFactory::CreateStaticActor(const physx::PxTransform& transform)
	{
		StaticActor* actor = new StaticActor(GenerateId());
		actor->m_currentPhysxActor = m_physics->createRigidStatic(transform);
		return actor;
	}

	Actor* ActorFactory::CreateDynamicActor(const physx::PxTransform& transform)
	{
		DynamicActor* actor = new DynamicActor(GenerateId());
		actor->m_currentPhysxActor = m_physics->createRigidDynamic(transform);
		return actor;
	}

	Actor* ActorFactory::CreateKinematicActor(const physx::PxTransform& transform)
	{
		DynamicActor* actor = new DynamicActor(GenerateId());
		auto dynamicPhysxActor = m_physics->createRigidDynamic(transform);
		dynamicPhysxActor->setRigidBodyFlags(physx::PxRigidBodyFlag::eKINEMATIC);
		actor->m_currentPhysxActor = dynamicPhysxActor;
		return actor;
	}

	Actor* ActorFactory::CreateClothActor(physx::PxU32*& quads, const physx::PxTransform& transform, const physx::PxVec2& size, physx::PxU32 width, physx::PxU32 height)
	{
		ClothActor* actor = new ClothActor(GenerateId());

		physx::PxReal w_step = size.x / width;
		physx::PxReal h_step = size.y / height;

		physx::PxClothParticle* vertices = new physx::PxClothParticle[(width + 1) * (height + 1) * 4];
		quads = new physx::PxU32[width * height * 4];

		// Create virtual particles
		for (physx::PxU32 j = 0; j < (height + 1); j++)
		{
			for (physx::PxU32 i = 0; i < (width + 1); i++)
			{
				physx::PxU32 offset = i + j * (width + 1);
				vertices[offset].pos = physx::PxVec3(w_step * i, 0.f, h_step * j);
				vertices[offset].invWeight = 1.f;
			}

			for (physx::PxU32 j = 0; j < height; j++)
			{
				for (physx::PxU32 i = 0; i < width; i++)
				{
					physx::PxU32 offset = (i + j * width) * 4;
					quads[offset + 0] = (i + 0) + (j + 0) * (width + 1);
					quads[offset + 1] = (i + 1) + (j + 0) * (width + 1);
					quads[offset + 2] = (i + 1) + (j + 1) * (width + 1);
					quads[offset + 3] = (i + 0) + (j + 1) * (width + 1);
				}
			}
		}

		physx::PxClothMeshDesc meshDesc;
		meshDesc.points.data = vertices;
		meshDesc.points.count = (width + 1) * (height + 1);
		meshDesc.points.stride = sizeof(physx::PxClothParticle);

		meshDesc.invMasses.data = &vertices->invWeight;
		meshDesc.invMasses.count = (width + 1) * (height + 1);
		meshDesc.invMasses.stride = sizeof(physx::PxClothParticle);

		meshDesc.quads.data = quads;
		meshDesc.quads.count = width * height;
		meshDesc.quads.stride = sizeof(physx::PxU32) * 4;

		physx::PxClothFabric* fabric = physx::PxClothFabricCreate(*m_physics, meshDesc, physx::PxVec3(0, -1, 0));

		auto physxActor = m_physics->createCloth(transform, *fabric, vertices, physx::PxClothFlag::eSCENE_COLLISION | physx::PxClothFlag::eSWEPT_CONTACT);

		actor->m_currentPhysxActor = physxActor;
		return actor;
	}
}

