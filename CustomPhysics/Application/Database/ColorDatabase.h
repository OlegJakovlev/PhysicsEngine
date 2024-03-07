#pragma once

#ifndef ColorDatabase_H
#define ColorDatabase_H

#include "../../PhysicsEngine/Databases/Database.h"
#include <PxPhysicsAPI.h>

namespace CustomApplication
{
	// TODO: Ideally create similar class to PhysicsEngine::Database and remove solution dependency
	class ColorDatabase : public PhysicsEngine::Database<physx::PxVec3>
	{
	public:
		bool Init() override;
		ColorDatabase::QueryResult ColorDatabase::AddEntry(uint32_t key, const physx::PxVec3& colorData);
	};
}

#endif