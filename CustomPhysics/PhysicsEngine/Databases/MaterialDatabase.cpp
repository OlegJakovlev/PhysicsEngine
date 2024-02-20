#include "MaterialDatabase.h"
#include "../Utility/Hashing.h"

namespace PhysicsEngine
{
	bool MaterialDatabase::Init()
	{
		return false;
	}

	bool MaterialDatabase::Init(const physx::PxPhysics* physics)
	{
		m_physics = const_cast<physx::PxPhysics*>(physics);
		AddEntry(CRC32_STR("Default"), physx::PxVec3(0.5, 0.5, 0.5));
		return true;
	}

	MaterialDatabase::QueryResult MaterialDatabase::AddEntry(uint32_t key, const physx::PxVec3& materialData)
	{
		QueryResult result;

		auto it = m_database.find(key);

		// There is identical entry
		if (it != m_database.end())
		{
			result.success = false;
			result.data = nullptr;
			return result;
		}

		physx::PxMaterial* newMaterial = m_physics->createMaterial(materialData.x, materialData.y, materialData.z);

		m_database[key] = newMaterial;
		result.success = true;
		result.data = &m_database[key];

		return result;
	}
}
