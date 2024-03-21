#include "AppVoidCallbackEntry.h"

namespace CustomApplication
{
	AppVoidCallbackEntry::AppVoidCallbackEntry(void* objectRef, AppVoidCallback callback) : m_objectRef(objectRef), m_callback(callback)
	{
	}
}