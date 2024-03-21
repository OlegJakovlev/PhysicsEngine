#pragma once

#ifndef KeyCallbackEntry_H
#define KeyCallbackEntry_H

#include <functional>

namespace CustomApplication
{
	struct KeyCallbackEntry
	{
		typedef std::function<void(float dt)> KeyCallback;

		void* m_objectRef;
		KeyCallback m_callback;

		KeyCallbackEntry(void* objectRef, KeyCallback callback);

		bool operator==(const KeyCallbackEntry& other) const
		{
			return m_objectRef == other.m_objectRef;
		}
	};
}

namespace std
{
	template<>
	struct hash<CustomApplication::KeyCallbackEntry>
	{
		std::size_t operator()(const CustomApplication::KeyCallbackEntry& entry) const noexcept
		{
			return std::hash<void*>{}(entry.m_objectRef);
		}
	};
}

#endif