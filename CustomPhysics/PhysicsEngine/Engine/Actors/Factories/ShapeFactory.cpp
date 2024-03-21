#include "ShapeFactory.h"
#include "../../../GlobalDefine.h"
#include "../../../Utility/Hashing.h"
#include "../../PhysicsEngine.h"

namespace PhysicsEngine
{
	physx::PxShape* ShapeFactory::CreateShapeInternal(StaticActor* actor, const physx::PxGeometry* geometry, const uint32_t materialKey, const physx::PxShape* src) const
	{
		physx::PxActor* physxActor = const_cast<physx::PxActor*>(actor->GetCurrentPhysxActor());
		physx::PxRigidStatic* rigidActor = physxActor->is<physx::PxRigidStatic>();

		if (!rigidActor)
		{
			printf("Actor casting to PxRigidStatic failed!");
			return nullptr;
		}

		// Material
		const physx::PxMaterial* material;
		QueryMaterial(materialKey, material);

		physx::PxShape* result = rigidActor->createShape(*geometry, *material);

		if (!result)
		{
			printf("CreateShape for StaticActor has failed!");
			return nullptr;
		}

		if (src)
		{
			result->setQueryFilterData(src->getQueryFilterData());
			result->setSimulationFilterData(src->getSimulationFilterData());
		}

		return result;
	}

	physx::PxShape* ShapeFactory::CreateShapeInternal(DynamicActor* actor, const physx::PxGeometry* geometry, const uint32_t materialKey, const physx::PxShape* src) const
	{
		physx::PxActor* physxActor = const_cast<physx::PxActor*>(actor->GetCurrentPhysxActor());
		physx::PxRigidDynamic* rigidActor = physxActor->is<physx::PxRigidDynamic>();
		if (!rigidActor)
		{
			printf("Actor casting to RigidActor failed!");
			return nullptr;
		}

		bool isKinematic = rigidActor->getRigidBodyFlags() & physx::PxRigidBodyFlag::eKINEMATIC;

		uint32_t geometryType = geometry->getType();
		if (isKinematic)
		{
			if (geometryType == physx::PxGeometryType::Enum::eTRIANGLEMESH)
			{
				printf("Incorrect geometry provided for dynamic kinematic actor! It can not be plane!");
				return nullptr;
			}
		}
		else
		{
			if (geometryType == physx::PxGeometryType::Enum::ePLANE || geometryType == physx::PxGeometryType::Enum::eHEIGHTFIELD || geometryType == physx::PxGeometryType::Enum::eTRIANGLEMESH)
			{
				printf("Incorrect geometry provided for dynamic actor! It can not be plane, heightfield or triangle mesh!");
				return nullptr;
			}
		}

		// Material
		const physx::PxMaterial* material;
		QueryMaterial(materialKey, material);

		physx::PxShape* result = rigidActor->createShape(*geometry, *material);
		if (!result)
		{
			printf("CreateShape for DynamicActor has failed!");
			return nullptr;
		}

		if (src)
		{
			result->setQueryFilterData(src->getQueryFilterData());
			result->setSimulationFilterData(src->getSimulationFilterData());
		}

		return result;
	}

	bool ShapeFactory::Init(const MaterialDatabase* database)
	{
		m_materialDatabase = database;
		return true;
	}

#ifdef PHYSICS_DEBUG_MODE
	void ShapeFactory::CreateShapeDebug(Actor* actor, const physx::PxGeometry* geometry, const uint32_t materialKey) const
	{
		Actor* castedActor = static_cast<Actor*>(actor);
		if (!castedActor)
		{
			std::printf("Create shape casting to Actor* failed!");
			return;
		}

		ActorType underlayingType = castedActor->GetType();

		if (underlayingType == ActorType::Dynamic)
		{
			DynamicActor* dynamicActor = (DynamicActor*) castedActor;
			CreateShapeInternal(dynamicActor, geometry, materialKey);
			return;
		}

		if (underlayingType == ActorType::Static)
		{
			StaticActor* staticActor = (StaticActor*) castedActor;
			CreateShapeInternal(staticActor, geometry, materialKey);
			return;
		}

		std::printf("ShapeCreator::CreateShape Unknown underlaying type!");
	}
#endif

	void ShapeFactory::CreateShape(void* actor, const physx::PxGeometry* geometry, const uint32_t materialKey, const physx::PxShape* src) const
	{
		Actor* castedActor = static_cast<Actor*>(actor);

		if (!castedActor)
		{
			std::printf("Create shape casting to Actor* failed!");
			return;
		}

		ActorType underlayingType = castedActor->GetType();

		if (underlayingType == ActorType::Dynamic)
		{
			DynamicActor* dynamicActor = (DynamicActor*) castedActor;
			CreateShapeInternal(dynamicActor, geometry, materialKey, src);
			return;
		}

		if (underlayingType == ActorType::Static)
		{
			StaticActor* staticActor = (StaticActor*) castedActor;
			CreateShapeInternal(staticActor, geometry, materialKey, src);
			return;
		}

		std::printf("ShapeCreator::CreateShape Unknown underlaying type!");
	}

	void ShapeFactory::CreateTrigger(void* actor, const physx::PxGeometry* geometry, const physx::PxShape* src) const
	{
		Actor* castedActor = static_cast<Actor*>(actor);
		if (!castedActor)
		{
			std::printf("CreateTrigger casting failed!");
			return;
		}

		physx::PxShape* shape = nullptr;
		ActorType underlayingType = castedActor->GetType();

		if (underlayingType == ActorType::Dynamic)
		{
			DynamicActor* dynamicActor = (DynamicActor*) castedActor;
			shape = CreateShapeInternal(dynamicActor, geometry, CRC32_STR("Default"), src);
		}

		if (underlayingType == ActorType::Static)
		{
			StaticActor* staticActor = (StaticActor*) castedActor;
			shape = CreateShapeInternal(staticActor, geometry, CRC32_STR("Default"), src);
		}

		if (!shape)
		{
			std::printf("CreateTrigger - Shape was not created");
			return;
		}

		shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
		shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
	}

	void ShapeFactory::QueryMaterial(const uint32_t materialKey, const physx::PxMaterial*& material) const
	{
		MaterialDatabase::QueryResult result = m_materialDatabase->GetEntry(materialKey);

		if (!result.success)
		{
			printf("Material with key [%i] was not found. Taking default material", materialKey);

			MaterialDatabase::QueryResult defaultMaterialQuery = m_materialDatabase->GetEntry(CRC32_STR("Default"));
			material = *defaultMaterialQuery.data;
		}
		else
		{
			material = *result.data;
		}
	}
}

