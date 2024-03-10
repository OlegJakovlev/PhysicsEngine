#pragma once

#ifndef VoidCallbackEntry_H
#define VoidCallbackEntry_H

#include <functional>

namespace PhysicsEngine
{
	struct VoidCallbackEntry
	{
		typedef std::function<void()> VoidCallback;

		void* m_objectRef;
		VoidCallback m_callback;

		VoidCallbackEntry(void* objectRef, VoidCallback callback);

        bool operator==(const VoidCallbackEntry& other) const
        {
            return m_objectRef == other.m_objectRef;
        }
	};
}

namespace std
{
    template<>
    struct hash<PhysicsEngine::VoidCallbackEntry>
    {
        std::size_t operator()(const PhysicsEngine::VoidCallbackEntry & entry) const noexcept
        {
            return std::hash<void*>{}(entry.m_objectRef);
        }
    };
}

#endif