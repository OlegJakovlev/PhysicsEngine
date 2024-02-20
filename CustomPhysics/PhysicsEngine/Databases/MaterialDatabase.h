#pragma once

#include "Database.h"
#include "PxPhysicsAPI.h"

class MaterialDatabase : public Database<physx::PxMaterial*>
{
private:
	physx::PxPhysics* m_physics;

public:
	bool Init() override;
	bool Init(const physx::PxPhysics* physics);

	// TODO: API Exposed
	QueryResult AddEntry(uint32_t key, const physx::PxVec3& materialData);
};

