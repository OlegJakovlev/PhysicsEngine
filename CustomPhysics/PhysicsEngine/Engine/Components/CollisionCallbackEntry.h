#pragma once

#ifndef CollisionCallbackEntry_H
#define CollisionCallbackEntry_H

#include <functional>

namespace PhysicsEngine
{
	struct CollisionCallbackEntry
	{
		typedef std::function<void(void*, void*, void*, void*)> CollisionCallback;

        void* m_objectRef;
		CollisionCallback m_callback;

		CollisionCallbackEntry(void* objectRef, CollisionCallback callback);

        bool operator==(const CollisionCallbackEntry& other) const
        {
            return m_objectRef == other.m_objectRef;
        }
	};
}

namespace std
{
    template<>
    struct hash<PhysicsEngine::CollisionCallbackEntry>
    {
        std::size_t operator()(const PhysicsEngine::CollisionCallbackEntry& entry) const noexcept
        {
            return std::hash<void*>{}(entry.m_objectRef);
        }
    };
}

#endif