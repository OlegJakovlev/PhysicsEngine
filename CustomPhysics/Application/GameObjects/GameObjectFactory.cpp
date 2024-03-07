#include "GameObjectFactory.h"
#include "StaticGameObject.h"
#include "DynamicGameObject.h"
#include "ClothGameObject.h"
#include "../../PhysicsEngine/Engine/Actors/StaticActor.h"
#include "../../PhysicsEngine/Engine/Actors/DynamicActor.h"
#include "../../PhysicsEngine/Engine/Actors/ClothActor.h"

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

    GameObject* GameObjectFactory::CreateStaticGameObject(const physx::PxTransform& transform, const GameObject::Layer layer)
    {
        StaticGameObject* staticObject = new StaticGameObject(GenerateId(), transform);
        PhysicsEngine::Actor* physicsActor = m_actorFactory->CreateStaticActor(staticObject->GetTransform());

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

    GameObject* GameObjectFactory::CreateDynamicGameObject(const physx::PxTransform& transform, const GameObject::Layer layer)
    {
        DynamicGameObject* dynamicObject = new DynamicGameObject(GenerateId(), transform);
        PhysicsEngine::Actor* physicsActor = m_actorFactory->CreateDynamicActor(dynamicObject->GetTransform());

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

    GameObject* GameObjectFactory::CreateKinematicGameObject(const physx::PxTransform& transform, const GameObject::Layer layer)
    {
        DynamicGameObject* dynamicObject = new DynamicGameObject(GenerateId(), transform);
        PhysicsEngine::Actor* physicsActor = m_actorFactory->CreateKinematicActor(dynamicObject->GetTransform());

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

    GameObject* GameObjectFactory::CreateClothGameObject(const physx::PxTransform& transform, const physx::PxVec2& size, const physx::PxU32 width, const physx::PxU32 height, const GameObject::Layer layer)
    {
        ClothGameObject* clothObject = new ClothGameObject(GenerateId(), transform);
        PhysicsEngine::Actor* physicsActor = m_actorFactory->CreateClothActor(transform, size, width, height);

        clothObject->SetPhysicsActor(physicsActor);
        physicsActor->SetCollisionLayer((uint32_t) layer);
        physicsActor->SetGameEnginePointerToPhysicsActor(clothObject->m_physicsActorPointer);

#ifdef APPLICATION_DEBUG_MODE
        clothObject->SetPhysicsActorDebug(physicsActor);

#ifdef PHYSICS_DEBUG_MODE
        physicsActor->SetGameEnginePointerToPhysicsActorDebug(clothObject->m_physicsActorPointerDebug);
#endif
#endif

        return clothObject;
    }
}