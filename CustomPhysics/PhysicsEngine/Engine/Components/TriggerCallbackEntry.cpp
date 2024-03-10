#include "TriggerCallbackEntry.h"

PhysicsEngine::TriggerCallbackEntry::TriggerCallbackEntry(void* objectRef, TriggerCallback callback) : m_objectRef(objectRef), m_callback(callback)
{
}
