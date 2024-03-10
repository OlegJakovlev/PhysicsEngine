#pragma once

#ifndef TriggerCallbackEntry_H
#define TriggerCallbackEntry_H

#include <functional>

namespace PhysicsEngine
{
	struct TriggerCallbackEntry
	{
		typedef std::function<void(void*, void*, void*, void*)> TriggerCallback;

        void* m_objectRef;
		TriggerCallback m_callback;

		TriggerCallbackEntry(void* m_objectRef, TriggerCallback callback);

        bool operator==(const TriggerCallbackEntry& other) const
        {
            return m_objectRef == other.m_objectRef;
        }
	};
}

namespace std
{
    template<>
    struct hash<PhysicsEngine::TriggerCallbackEntry>
    {
        std::size_t operator()(const PhysicsEngine::TriggerCallbackEntry& entry) const noexcept
        {
            return std::hash<void*>{}(entry.m_objectRef);
        }
    };
}

#endif