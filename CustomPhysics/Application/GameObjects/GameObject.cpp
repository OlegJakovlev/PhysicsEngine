#include "GameObject.h"

namespace CustomApplication
{
	GameObject::GameObject(const uint64_t id, const Type type, physx::PxTransform& transform)
	{
		m_actorID = id;
		m_type = type;
		m_transform = transform;
	}

	void GameObject::SetPhysicsActor(void* actor)
	{
		m_physicsActor = actor;
	}

	const void* GameObject::GetPhysicsActor() const
	{
		return m_physicsActor;
	}

	const RenderData* GameObject::GetRenderData() const
	{
		return m_renderData;
	}

	const physx::PxTransform& GameObject::GetTransform() const
	{
		return m_transform;
	}

	const GameObject::Type GameObject::GetType() const
	{
		return m_type;
	}
}