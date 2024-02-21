#include "GameObjectFactory.h"
#include "StaticGameObject.h"
#include "DynamicGameObject.h"
#include "../../PhysicsEngine/Engine/Actors/StaticActor.h"
#include "../../PhysicsEngine/Engine/Actors/DynamicActor.h"

namespace CustomApplication
{
    std::atomic<uint64_t> GameObjectFactory::s_lastId = 0;

    uint64_t GameObjectFactory::GenerateId()
    {
        return s_lastId++;
    }

    void GameObjectFactory::Init(PhysicsEngine::ActorFactory* factory)
    {
        m_actorFactory = factory;
    }

    GameObject* GameObjectFactory::CreateStaticActor(physx::PxTransform& transform, GameObject::Layer layer)
    {
        StaticGameObject* staticObject = new StaticGameObject(GenerateId(), transform);
        PhysicsEngine::Actor* physicsActor = (PhysicsEngine::StaticActor*) m_actorFactory->CreateStaticActor(staticObject->GetTransform());

        staticObject->SetPhysicsActor(physicsActor);
        physicsActor->SetCollisionLayer((uint32_t) layer);
        physicsActor->SetGameEnginePointerToPhysicsActor(staticObject->m_physicsActorPointer);

#ifdef APPLICATION_DEBUG_MODE
        staticObject->SetPhysicsActorDebug(physicsActor);

#ifdef PHYSICS_DEBUG_MODE
        physicsActor->SetGameEnginePointerToPhysicsActorDebug(staticObject->m_physicsActorPointerDebug);
#endif
#endif

        return staticObject;
    }

    GameObject* GameObjectFactory::CreateDynamicActor(physx::PxTransform& transform, GameObject::Layer layer)
    {
        DynamicGameObject* dynamicObject = new DynamicGameObject(GenerateId(), transform);
        PhysicsEngine::Actor* physicsActor = (PhysicsEngine::DynamicActor*) m_actorFactory->CreateDynamicActor(dynamicObject->GetTransform());

        dynamicObject->SetPhysicsActor(physicsActor);
        physicsActor->SetCollisionLayer((uint32_t) layer);
        physicsActor->SetGameEnginePointerToPhysicsActor(dynamicObject->m_physicsActorPointer);

#ifdef APPLICATION_DEBUG_MODE
        dynamicObject->SetPhysicsActorDebug(physicsActor);

#ifdef PHYSICS_DEBUG_MODE
        physicsActor->SetGameEnginePointerToPhysicsActorDebug(dynamicObject->m_physicsActorPointerDebug);
#endif
#endif

        return dynamicObject;
    }

    GameObject* GameObjectFactory::CreateKinematicActor(physx::PxTransform& transform, GameObject::Layer layer)
    {
        DynamicGameObject* dynamicObject = new DynamicGameObject(GenerateId(), transform);
        PhysicsEngine::Actor* physicsActor = (PhysicsEngine::DynamicActor*) m_actorFactory->CreateKinematicActor(dynamicObject->GetTransform());

        dynamicObject->SetPhysicsActor(physicsActor);
        physicsActor->SetCollisionLayer((uint32_t) layer);
        physicsActor->SetGameEnginePointerToPhysicsActor(dynamicObject->m_physicsActorPointer);

#ifdef APPLICATION_DEBUG_MODE
        dynamicObject->SetPhysicsActorDebug(physicsActor);

#ifdef PHYSICS_DEBUG_MODE
        physicsActor->SetGameEnginePointerToPhysicsActorDebug(dynamicObject->m_physicsActorPointerDebug);
#endif
#endif

        return dynamicObject;
    }
}