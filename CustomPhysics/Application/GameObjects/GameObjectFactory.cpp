#include "GameObjectFactory.h"
#include "StaticGameObject.h"
#include "DynamicGameObject.h"
#include "../../PhysicsEngine/Engine/Actors/StaticActor.h"
#include "../../PhysicsEngine/Engine/Actors/DynamicActor.h"

std::atomic<uint64_t> GameObjectFactory::s_lastId = 0;

uint64_t GameObjectFactory::GenerateId()
{
    return s_lastId++;
}

void GameObjectFactory::Init(ActorFactory* factory)
{
    m_actorFactory = factory;
}

GameObject* GameObjectFactory::CreateStaticActor(physx::PxTransform& transform, GameObject::Layer layer)
{
    StaticGameObject* staticObject = new StaticGameObject(GenerateId(), transform);
    StaticActor* physicsActor = (StaticActor*) m_actorFactory->CreateStaticActor(staticObject->GetTransform());

    staticObject->SetPhysicsActor(physicsActor);
    physicsActor->SetCollisionLayer(CollisionFilter::FilterGroup (layer));
    
    return staticObject;
}

GameObject* GameObjectFactory::CreateDynamicActor(physx::PxTransform& transform, GameObject::Layer layer)
{
    DynamicGameObject* dynamicObject = new DynamicGameObject(GenerateId(), transform);
    DynamicActor* dynamicExample = (DynamicActor*) m_actorFactory->CreateDynamicActor(dynamicObject->GetTransform());

    dynamicObject->SetPhysicsActor(dynamicExample);
    dynamicExample->SetCollisionLayer(CollisionFilter::FilterGroup (layer));

    return dynamicObject;
}

GameObject* GameObjectFactory::CreateKinematicActor(physx::PxTransform& transform, GameObject::Layer layer)
{
    DynamicGameObject* dynamicObject = new DynamicGameObject(GenerateId(), transform);
    DynamicActor* dynamicExample = (DynamicActor*) m_actorFactory->CreateKinematicActor(dynamicObject->GetTransform());

    dynamicObject->SetPhysicsActor(dynamicExample);
    dynamicExample->SetCollisionLayer(CollisionFilter::FilterGroup(layer));

    return dynamicObject;
}
