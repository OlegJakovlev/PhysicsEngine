#pragma once

#ifndef Camera_H
#define Camera_H

#include "foundation/PxTransform.h"

namespace CustomApplication
{
	class Camera
	{
	private:
		physx::PxVec3 m_pos;
		physx::PxVec3 m_posInit;
		physx::PxVec3 m_dir;
		physx::PxVec3 m_dirInit;
		physx::PxReal m_speed;
		physx::PxReal m_speedInit;

	public:
		Camera(const physx::PxVec3& _pos, const physx::PxVec3& _dir, const physx::PxReal _speed);
		void Reset();
		void Motion(int dx, int dy, physx::PxReal dt);
		void AnalogMove(float x, float y);
		physx::PxVec3 GetPos() const;
		physx::PxVec3 GetDir() const;
		physx::PxTransform GetTransform() const;
		void MoveForward(physx::PxReal dt);
		void MoveBackward(physx::PxReal dt);
		void MoveLeft(physx::PxReal dt);
		void MoveRight(physx::PxReal dt);
		void MoveUp(physx::PxReal dt);
		void MoveDown(physx::PxReal dt);
	};
}


#endif