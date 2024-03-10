#include "VoidCallbackEntry.h"

PhysicsEngine::VoidCallbackEntry::VoidCallbackEntry(void* objectRef, VoidCallback callback) : m_objectRef(objectRef), m_callback(callback)
{
}
