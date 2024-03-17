#pragma once

#ifndef MouseCallbackEntry_H
#define MouseCallbackEntry_H

#include <functional>

namespace CustomApplication
{
	struct MouseMoveCallbackEntry
	{
		typedef std::function<void(int dx, int dy, int x, int y, float dt)> MouseMoveCallback;

		void* m_objectRef;
		MouseMoveCallback m_callback;

		MouseMoveCallbackEntry(void* objectRef, MouseMoveCallback callback);

		bool operator==(const MouseMoveCallbackEntry& other) const
		{
			return m_objectRef == other.m_objectRef;
		}
	};
}

namespace std
{
	template<>
	struct hash<CustomApplication::MouseMoveCallbackEntry>
	{
		std::size_t operator()(const CustomApplication::MouseMoveCallbackEntry& entry) const noexcept
		{
			return std::hash<void*>{}(entry.m_objectRef);
		}
	};
}

#endif