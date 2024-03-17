#include "GameObjectFactory.h"
#include "StaticGameObject.h"
#include "DynamicGameObject.h"
#include "ClothGameObject.h"
#include "VehicleGameObject.h"
#include "../../PhysicsEngine/Engine/Actors/StaticActor.h"
#include "../../PhysicsEngine/Engine/Actors/DynamicActor.h"
#include "../../PhysicsEngine/Engine/Actors/ClothActor.h"
#include "../../PhysicsEngine/Engine/Actors/VehicleActor.h"
#include "CustomRenderGameObject.h"

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
        StaticGameObject* staticObject = new StaticGameObject(GenerateId());
        PhysicsEngine::Actor* physicsActor = m_actorFactory->CreateStaticActor(transform);

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
        DynamicGameObject* dynamicObject = new DynamicGameObject(GenerateId());
        PhysicsEngine::Actor* physicsActor = m_actorFactory->CreateDynamicActor(transform);

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
        DynamicGameObject* dynamicObject = new DynamicGameObject(GenerateId());
        PhysicsEngine::Actor* physicsActor = m_actorFactory->CreateKinematicActor(transform);

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

    GameObject* GameObjectFactory::CreateClothGameObject(const physx::PxTransform& transform, const physx::PxVec2& size, const physx::PxVec2& scale, const GameObject::Layer layer)
    {
        ClothGameObject* clothObject = new ClothGameObject(GenerateId());
        PhysicsEngine::Actor* physicsActor = m_actorFactory->CreateClothActor(transform, size, scale);

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

    GameObject* GameObjectFactory::CreateVehicleGameObject(const physx::PxTransform& transform,
                                                           const PhysicsEngine::VehicleData* configData,
                                                           const GameObject::Layer layer)
    {
        VehicleGameObject* vehicleObject = new VehicleGameObject(GenerateId());

        PhysicsEngine::Actor* physicsActor = m_actorFactory->CreateVehicleActor(transform, configData);

        vehicleObject->SetPhysicsActor(physicsActor);
        physicsActor->SetCollisionLayer((uint32_t) layer);
        physicsActor->SetGameEnginePointerToPhysicsActor(vehicleObject->m_physicsActorPointer);

#ifdef APPLICATION_DEBUG_MODE
        vehicleObject->SetPhysicsActorDebug(physicsActor);

#ifdef PHYSICS_DEBUG_MODE
        physicsActor->SetGameEnginePointerToPhysicsActorDebug(vehicleObject->m_physicsActorPointerDebug);
#endif
#endif

        return vehicleObject;
    }

    GameObject* GameObjectFactory::CreateCustomRenderGameObject()
    {
        CustomRenderGameObject* customRenderObject = new CustomRenderGameObject(GenerateId());
        return customRenderObject;
    }
}