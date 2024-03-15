#pragma once

#ifndef VehicleGameObject_H
#define VehicleGameObject_H

#include "GameObject.h"

namespace CustomApplication
{
	class VehicleGameObject : public GameObject
	{
		friend class GameObjectFactory;

	private:
		VehicleGameObject() = delete;
		VehicleGameObject(const uint64_t id);
	};
}

#endif