#include "GameObject.h"

namespace CustomApplication
{
	GameObject::GameObject(const uint64_t id, const Type type)
	{
		m_actorID = id;
		m_type = type;
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

	RenderData& GameObject::GetRenderData()
	{
		return m_renderData;
	}

	const RenderData& GameObject::GetRenderData() const
	{
		return m_renderData;
	}

	const GameObject::Type GameObject::GetType() const
	{
		return m_type;
	}
}