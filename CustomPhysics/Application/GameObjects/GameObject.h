#pragma once

#ifndef GameObject_H
#define GameObject_H

#include "PxPhysicsAPI.h"
#include "../Rendering/RenderData.h"
#include "../../PhysicsEngine/Engine/Actors/Actor.h"
#include "../GlobalDefine.h"

namespace CustomApplication
{
	class GameObject
	{
	public:
		enum class Layer : uint32_t
		{
			Layer_Default = (1 << 0),
			Layer_1 = (1 << 1),
			Layer_2 = (1 << 2),
			Layer_3 = (1 << 3),
			Layer_4 = (1 << 4),
			Layer_5 = (1 << 5),
			Layer_6 = (1 << 6),
			Layer_7 = (1 << 7),
			Layer_8 = (1 << 8),
			Layer_9 = (1 << 9),
			Layer_10 = (1 << 10),
			Layer_11 = (1 << 11),
			Layer_12 = (1 << 12),
			Layer_13 = (1 << 13),
			Layer_14 = (1 << 14),
			Layer_15 = (1 << 15),
			Layer_16 = (1 << 16),
			Layer_17 = (1 << 17),
			Layer_18 = (1 << 18),
			Layer_19 = (1 << 19),
			Layer_20 = (1 << 20),
			Layer_21 = (1 << 21),
			Layer_22 = (1 << 22),
			Layer_23 = (1 << 23),
			Layer_24 = (1 << 24),
			Layer_25 = (1 << 25),
			Layer_26 = (1 << 26),
			Layer_27 = (1 << 27),
			Layer_28 = (1 << 28),
			Layer_29 = (1 << 29),
			Layer_30 = (1 << 30),
			Layer_31 = (1 << 31),
		};

		enum Type
		{
			Static,
			Dynamic,
		};

	protected:
		uint64_t m_actorID;
		physx::PxTransform m_transform;
		Type m_type;

		void* m_physicsActor;
		void** m_physicsActorPointer;

#ifdef APPLICATION_DEBUG_MODE
		PhysicsEngine::Actor* m_physicsActorDebug;
		PhysicsEngine::Actor** m_physicsActorPointerDebug;
#endif

		RenderData* m_renderData;

		GameObject() = delete;
		GameObject(const uint64_t id, const Type type, physx::PxTransform& transform);

	public:
		void** GetPhysicsActorPointer() const;
		void SetPhysicsActor(void* actor);

#ifdef APPLICATION_DEBUG_MODE
		PhysicsEngine::Actor** GetPhysicsActorPointerDebug() const;
		void SetPhysicsActorDebug(PhysicsEngine::Actor* actor);
#endif

		const RenderData* GetRenderData() const;
		const physx::PxTransform& GetTransform() const;
		const Type GetType() const;
	};
}

#endif