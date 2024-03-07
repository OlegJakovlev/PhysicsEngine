#include "ColorDatabase.h"
#include "../Utility/Hashing.h"

namespace CustomApplication
{
	bool ColorDatabase::Init()
	{
		AddEntry(CRC32_STR("Default"), physx::PxVec3(0.8f, 0.8f, 0.8f));
		return true;
	}

	ColorDatabase::QueryResult ColorDatabase::AddEntry(uint32_t key, const physx::PxVec3& colorData)
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

		m_database[key] = colorData;
		result.success = true;
		result.data = &m_database[key];

		return result;
	}
}
