#include "CollisionCallbackEntry.h"

PhysicsEngine::CollisionCallbackEntry::CollisionCallbackEntry(void* objectRef, CollisionCallback callback) : m_objectRef(objectRef), m_callback(callback)
{
}
