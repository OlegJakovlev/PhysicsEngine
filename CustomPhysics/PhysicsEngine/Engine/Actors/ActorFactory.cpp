#include "ActorFactory.h"
#include "StaticActor.h"
#include "DynamicActor.h"
#include <vector>

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
		auto staticPhysxActor = m_physics->createRigidStatic(transform);

		staticPhysxActor->setActorFlag(physx::PxActorFlag::eSEND_SLEEP_NOTIFIES, true);

		// Link actor to physx to have an access from collision calls 
		staticPhysxActor->userData = actor;
		actor->m_currentPhysxActor = staticPhysxActor;

		return actor;
	}

	Actor* ActorFactory::CreateDynamicActor(const physx::PxTransform& transform)
	{
		DynamicActor* actor = new DynamicActor(GenerateId());
		auto dynamicPhysxActor = m_physics->createRigidDynamic(transform);

		dynamicPhysxActor->setActorFlag(physx::PxActorFlag::eSEND_SLEEP_NOTIFIES, true);

		// Link actor to physx to have an access from collision calls 
		dynamicPhysxActor->userData = actor;
		actor->m_currentPhysxActor = dynamicPhysxActor;

		return actor;
	}

	Actor* ActorFactory::CreateKinematicActor(const physx::PxTransform& transform)
	{
		DynamicActor* actor = new DynamicActor(GenerateId());
		auto dynamicPhysxActor = m_physics->createRigidDynamic(transform);

		dynamicPhysxActor->setActorFlag(physx::PxActorFlag::eSEND_SLEEP_NOTIFIES, true);
		dynamicPhysxActor->setRigidBodyFlags(physx::PxRigidBodyFlag::eKINEMATIC);

		// Link actor to physx to have an access from collision calls 
		dynamicPhysxActor->userData = actor;
		actor->m_currentPhysxActor = dynamicPhysxActor;

		return actor;
	}

	Actor* ActorFactory::CreateClothActor(const physx::PxTransform& transform, const physx::PxVec2& size, const physx::PxVec2& scale)
	{
		ClothActor* actor = new ClothActor(GenerateId(), size, scale);

		const physx::PxU32 width = scale.x;
		const physx::PxU32 height = scale.y;

		physx::PxReal w_step = size.x / width;
		physx::PxReal h_step = size.y / height;

		physx::PxClothParticle* vertices = new physx::PxClothParticle[(width + 1) * (height + 1) * 4];
		physx::PxU32* quadContainer = new physx::PxU32[width * height * 4];
		physx::PxU32 quadAmount = width * height * 4;

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
					quadContainer[offset + 0] = (i + 0) + (j + 0) * (width + 1);
					quadContainer[offset + 1] = (i + 1) + (j + 0) * (width + 1);
					quadContainer[offset + 2] = (i + 1) + (j + 1) * (width + 1);
					quadContainer[offset + 3] = (i + 0) + (j + 1) * (width + 1);
				}
			}
		}

		physx::PxClothMeshDesc& meshDesc = actor->m_meshDesc;

		meshDesc.points.data = vertices;
		meshDesc.points.count = (width + 1) * (height + 1);
		meshDesc.points.stride = sizeof(physx::PxClothParticle);

		meshDesc.invMasses.data = &vertices->invWeight;
		meshDesc.invMasses.count = (width + 1) * (height + 1);
		meshDesc.invMasses.stride = sizeof(physx::PxClothParticle);

		meshDesc.quads.data = quadContainer;
		meshDesc.quads.count = width * height;
		meshDesc.quads.stride = sizeof(physx::PxU32) * 4;

		physx::PxClothFabric* fabric = physx::PxClothFabricCreate(*m_physics, meshDesc, physx::PxVec3(0, 0, 0));

		auto physxActor = m_physics->createCloth(transform, *fabric, vertices,
												 physx::PxClothFlag::eSCENE_COLLISION |
												 physx::PxClothFlag::eSWEPT_CONTACT);
		
		// https://docs.nvidia.com/gameworks/content/gameworkslibrary/physx/guide/Manual/Cloth.html#specifying-collision-shapes
		// TODO: Think about custom stretch configs (each constraint type)
		// physxActor.setStretchConfig(PxClothFabricPhaseType::eVERTICAL, PxClothStretchConfig(1.0f));
		// physxActor.setStretchConfig(PxClothFabricPhaseType::eHORIZONTAL, PxClothStretchConfig(0.9f));
		// physxActor.setStretchConfig(PxClothFabricPhaseType::eSHEARING, PxClothStretchConfig(0.75f));
		// physxActor.setStretchConfig(PxClothFabricPhaseType::eBENDING, PxClothStretchConfig(0.5f));

		// TODO: Think about virtual particles (used only for collision phase)
		// static PxVec3 weights[] = {}
		// cloth.setVirtualParticles(numFaces*4, indices, 2, weights);

		// Link actor to physx to have an access from collision calls 
		physxActor->userData = actor;
		actor->m_currentPhysxActor = physxActor;

		return actor;
	}

	Actor* ActorFactory::CloneClothActor(const ClothActor* originalActor)
	{
		auto originalClothPhysxActor = ((physx::PxCloth*) originalActor->GetCurrentPhysxActor());

		ClothActor* clone = new ClothActor(GenerateId(), originalActor->m_size, originalActor->m_scale);
		clone->m_meshDesc = originalActor->m_meshDesc;

		const physx::PxU32 width = originalActor->m_scale.x;
		const physx::PxU32 height = originalActor->m_scale.y;
		physx::PxClothParticle* vertices;

		// Make sure the particles share the same list
		physx::PxClothParticleData* particleData = originalClothPhysxActor->lockParticleData();
		vertices = particleData->particles;
		particleData->unlock();

		auto clothPhysxActor = m_physics->createCloth(originalClothPhysxActor->getGlobalPose(),
													  *originalClothPhysxActor->getFabric(), vertices,
													  physx::PxClothFlag::eSCENE_COLLISION |
													  physx::PxClothFlag::eSWEPT_CONTACT);

		clothPhysxActor->setSimulationFilterData(originalClothPhysxActor->getSimulationFilterData());

		// Link actor to physx to have an access from collision calls 
		clothPhysxActor->userData = clone;
		clone->m_currentPhysxActor = clothPhysxActor;

		return clone;
	}
}

