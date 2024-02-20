#include "Camera.h"
#include "foundation/PxMat33.h"

namespace CustomApplication
{
	Camera::Camera(const physx::PxVec3& _pos, const physx::PxVec3& _dir, physx::PxReal _speed)
	{
		m_posInit = _pos;
		m_dirInit = _dir.getNormalized();
		m_speedInit = _speed;
		Reset();
	}

	void Camera::Reset()
	{
		m_pos = m_posInit;
		m_dir = m_dirInit;
		m_speed = m_speedInit;
	}

	void Camera::Motion(int dx, int dy, physx::PxReal dt)
	{
		physx::PxVec3 viewY = m_dir.cross(physx::PxVec3(0, 1, 0)).getNormalized();

		physx::PxQuat qx(physx::PxPi * dx * m_speed * dt / 180.0f,
						 physx::PxVec3(0, 1, 0));
		m_dir = qx.rotate(m_dir);

		physx::PxQuat qy(physx::PxPi * dy * m_speed * dt / 180.0f, viewY);
		m_dir = qy.rotate(m_dir);

		m_dir.normalize();
	}

	void Camera::AnalogMove(float x, float y)
	{
		physx::PxVec3 viewY = m_dir.cross(physx::PxVec3(0, 1, 0)).getNormalized();
		m_pos += m_dir * y;
		m_pos += viewY * x;
	}

	physx::PxVec3 Camera::GetPos() const
	{
		return m_pos;
	}

	physx::PxVec3 Camera::GetDir() const
	{
		return m_dir;
	}

	physx::PxTransform Camera::GetTransform() const
	{
		physx::PxVec3 viewY = m_dir.cross(physx::PxVec3(0, 1, 0));

		if (viewY.normalize() < 1e-6f)
		{
			return physx::PxTransform(m_pos);
		}

		physx::PxMat33 m(m_dir.cross(viewY), viewY, -m_dir);
		return physx::PxTransform(m_pos, physx::PxQuat(m));
	}

	void Camera::MoveForward(physx::PxReal dt)
	{
		m_pos += m_dir * m_speed * dt;
	}

	void Camera::MoveBackward(physx::PxReal dt)
	{
		m_pos -= m_dir * m_speed * dt;
	}

	void Camera::MoveLeft(physx::PxReal dt)
	{
		physx::PxVec3 viewY = m_dir.cross(physx::PxVec3(0, 1, 0)).getNormalized();
		m_pos -= viewY * m_speed * dt;
	}

	void Camera::MoveRight(physx::PxReal dt)
	{
		physx::PxVec3 viewY = m_dir.cross(physx::PxVec3(0, 1, 0)).getNormalized();
		m_pos += viewY * m_speed * dt;
	}

	void Camera::MoveUp(physx::PxReal dt)
	{
		m_pos += physx::PxVec3(0, 1, 0) * m_speed * dt;
	}

	void Camera::MoveDown(physx::PxReal dt)
	{
		m_pos -= physx::PxVec3(0, 1, 0) * m_speed * dt;
	}
}
