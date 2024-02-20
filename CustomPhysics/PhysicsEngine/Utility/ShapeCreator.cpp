#include "ShapeCreator.h"
#include "Hashing.h"
#include "../Engine/PhysicsEngine.h"

physx::PxShape* ShapeCreator::CreateShapeInternal(StaticActor* actor, const physx::PxGeometry* geometry, const uint32_t materialKey) const
{
	const physx::PxActor* physxActor = actor->GetPhysicsActor();
	physx::PxRigidStatic* rigidActor = const_cast<physx::PxRigidStatic*>(physxActor->is<physx::PxRigidStatic>());

	if (!rigidActor)
	{
		printf("Actor casting to PxRigidStatic failed!");
		return nullptr;
	}

	const physx::PxMaterial* material;
	QueryMaterial(materialKey, material);

	physx::PxShape* result = rigidActor->createShape(*geometry, *material);

	if (!result)
	{
		printf("CreateShape for StaticActor has failed!");
		return nullptr;
	}

	return result;
}

physx::PxShape* ShapeCreator::CreateShapeInternal(DynamicActor* actor, const physx::PxGeometry* geometry, const uint32_t materialKey) const
{
	physx::PxActor* physxActor = const_cast<physx::PxActor*>(actor->GetPhysicsActor());
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

	const physx::PxMaterial* material;
	QueryMaterial(materialKey, material);

	physx::PxShape* result = rigidActor->createShape(*geometry, *material);
	if (!result)
	{
		printf("CreateShape for DynamicActor has failed!");
		return nullptr;
	}

	return result;
}

bool ShapeCreator::Init(const MaterialDatabase* database)
{
	m_materialDatabase = database;
	return true;
}

void ShapeCreator::CreateShape(void* actor, const physx::PxGeometry* geometry, const uint32_t materialKey) const
{
	Actor* castedActor = static_cast<Actor*>(actor);
	if (!castedActor)
	{
		std::printf("Create shape casting to Actor* failed!");
		return;
	}

	Actor::Type underlayingType = castedActor->GetType();

	if (underlayingType == Actor::Type::Dynamic)
	{
		DynamicActor* dynamicActor = static_cast<DynamicActor*>(castedActor);
		CreateShapeInternal(dynamicActor, geometry, materialKey);
		return;
	}

	if (underlayingType == Actor::Type::Static)
	{
		StaticActor* staticActor = static_cast<StaticActor*>(castedActor);
		CreateShapeInternal(staticActor, geometry, materialKey);
		return;
	}

	std::printf("Unknown underlaying type!");
}

void ShapeCreator::CreateTrigger(void* actor, const physx::PxGeometry* geometry, const uint32_t materialKey) const
{
	Actor* castedActor = static_cast<Actor*>(actor);
	if (!castedActor)
	{
		std::printf("CreateTrigger casting failed!");
		return;
	}

	physx::PxShape* shape = nullptr;
	Actor::Type underlayingType = castedActor->GetType();

	if (underlayingType == Actor::Type::Dynamic)
	{
		DynamicActor* dynamicActor = static_cast<DynamicActor*>(castedActor);
		shape = CreateShapeInternal(dynamicActor, geometry, materialKey);
	}

	if (underlayingType == Actor::Type::Static)
	{
		StaticActor* staticActor = static_cast<StaticActor*>(castedActor);
		shape = CreateShapeInternal(staticActor, geometry, materialKey);
	}

	if (!shape)
	{
		std::printf("CreateTrigger - Shape was not created");
		return;
	}

	shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
	shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
}

void ShapeCreator::QueryMaterial(const uint32_t materialKey, const physx::PxMaterial*& material) const
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

