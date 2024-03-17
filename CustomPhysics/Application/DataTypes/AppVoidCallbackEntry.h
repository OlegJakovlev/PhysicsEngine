#pragma once

#ifndef AppVoidCallbackEntry_H
#define AppVoidCallbackEntry_H

#include <functional>

namespace CustomApplication
{
    struct AppVoidCallbackEntry
    {
        typedef std::function<void()> AppVoidCallback;

        void* m_objectRef;
        AppVoidCallback m_callback;

        AppVoidCallbackEntry(void* objectRef, AppVoidCallback callback);

        bool operator==(const AppVoidCallbackEntry& other) const
        {
            return m_objectRef == other.m_objectRef;
        }
    };
}

namespace std
{
    template<>
    struct hash<CustomApplication::AppVoidCallbackEntry>
    {
        std::size_t operator()(const CustomApplication::AppVoidCallbackEntry& entry) const noexcept
        {
            return std::hash<void*>{}(entry.m_objectRef);
        }
    };
}

#endif