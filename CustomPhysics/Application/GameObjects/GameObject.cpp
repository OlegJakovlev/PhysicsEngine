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
		m_physicsActorPointer = &m_physicsActor;
	}

	void** GameObject::GetPhysicsActorPointer() const
	{
		return m_physicsActorPointer;
	}

#ifdef APPLICATION_DEBUG_MODE
	void GameObject::SetPhysicsActorDebug(PhysicsEngine::Actor* actor)
	{
		m_physicsActorDebug = actor;
		m_physicsActorPointerDebug = &m_physicsActorDebug;
	}

	PhysicsEngine::Actor** GameObject::GetPhysicsActorPointerDebug() const
	{
		return m_physicsActorPointerDebug;
	}
#endif

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