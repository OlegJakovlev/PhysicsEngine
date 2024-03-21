#include "CustomSimulationEventCallback.h"
#include "PxRigidDynamic.h"
#include <iostream>
#include <vector>
#include "../Actors/Actor.h"

namespace PhysicsEngine
{
    void CustomSimulationEventCallback::onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count)
    {
    }

    void CustomSimulationEventCallback::onWake(physx::PxActor** actors, physx::PxU32 count)
    {
        for (physx::PxU32 i = 0; i < count; i++)
        {
            auto engineTriggerActor = (Actor*) actors[i]->userData;
            engineTriggerActor->OnWake();
        }
    }

    void CustomSimulationEventCallback::onSleep(physx::PxActor** actors, physx::PxU32 count)
    {
        for (physx::PxU32 i = 0; i < count; i++)
        {
            auto engineTriggerActor = (Actor*) actors[i]->userData;
            engineTriggerActor->OnSleep();
        }
    }

    // https://nvidia-omniverse.github.io/PhysX/physx/5.2.1/docs/Simulation.html
    // https://nvidia-omniverse.github.io/PhysX/physx/5.3.0/docs/AdvancedCollisionDetection.html
    void CustomSimulationEventCallback::onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs)
    {
        for (physx::PxU32 i = 0; i < nbPairs; i++)
        {
            const physx::PxContactPair& cp = pairs[i];

            // Extracting Additional Information
            physx::PxU32 contactCount = cp.contactCount;

            std::vector<ContactInfo*> contactsData(contactCount);

            if (contactCount)
            {
                std::vector<physx::PxContactPairPoint> contactPoints(contactCount);
                cp.extractContacts(&contactPoints[0], contactCount);

                for (physx::PxU32 j = 0; j < contactCount; j++)
                {
                    const physx::PxContactPairPoint& contactPoint = contactPoints[j];
                    contactsData[j] = new ContactInfo(contactPoint.position, contactPoint.impulse, contactPoint.normal);
                }
            }

            // Handling Contact Events
            auto firstActor = (Actor*) pairHeader.actors[0]->userData;
            auto otherActor = (Actor*) pairHeader.actors[1]->userData;

            if (cp.events & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
            {
                //std::printf("onContact::eNOTIFY_TOUCH_FOUND\n");
                firstActor->OnCollisionEnter(otherActor, contactsData);
                otherActor->OnCollisionEnter(firstActor, contactsData);
            }

            if (cp.events & physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS)
            {
                //std::printf("onContact::eNOTIFY_TOUCH_PERSISTS\n");
                firstActor->OnCollisionStay(otherActor, contactsData);
                otherActor->OnCollisionStay(firstActor, contactsData);
            }

            if (cp.events & physx::PxPairFlag::eNOTIFY_TOUCH_LOST)
            {
                //std::printf("onContact::eNOTIFY_TOUCH_LOST\n");
                firstActor->OnCollisionExit(otherActor, contactsData);
                otherActor->OnCollisionExit(firstActor, contactsData);
            }

            // Cleanup
            for (size_t j = 0; j < contactCount; j++)
            {
                delete contactsData[j];
            }

            contactsData.clear();
        }

    }

    // https://nvidia-omniverse.github.io/PhysX/physx/5.2.1/docs/RigidBodyCollision.html#collisionfiltering
    void CustomSimulationEventCallback::onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count)
    {
        for (physx::PxU32 i = 0; i < count; i++)
        {
            const physx::PxTriggerPair& tp = pairs[i];

            // Ignore pairs when shapes have been deleted
            if (tp.flags & (physx::PxTriggerPairFlag::eREMOVED_SHAPE_TRIGGER | physx::PxTriggerPairFlag::eREMOVED_SHAPE_OTHER))
            {
                continue;
            }

            // Handling Contact Events
            auto engineTriggerActor = (Actor*) tp.triggerActor->userData;
            auto engineOtherActor = (Actor*) tp.otherActor->userData;

            if (tp.status & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
            {
                //std::printf("onTrigger::eNOTIFY_TOUCH_FOUND\n");
                engineTriggerActor->OnTriggerEnter(tp.triggerShape, engineOtherActor, tp.otherShape);
            }

            if (tp.status & physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS)
            {
                //std::printf("onTrigger::eNOTIFY_TOUCH_PERSISTS\n");
                engineTriggerActor->OnTriggerStay(tp.triggerShape, engineOtherActor, tp.otherShape);
            }

            if (tp.status & physx::PxPairFlag::eNOTIFY_TOUCH_LOST)
            {
                //std::printf("onTrigger::eNOTIFY_TOUCH_LOST\n");
                engineTriggerActor->OnTriggerExit(tp.triggerShape, engineOtherActor, tp.otherShape);
            }
        }

    }

    void CustomSimulationEventCallback::onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count)
    {
    }
}
