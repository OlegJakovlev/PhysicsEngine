#include "KeyCallbackEntry.h"

namespace CustomApplication
{
	KeyCallbackEntry::KeyCallbackEntry(void* objectRef, KeyCallback callback) : m_objectRef(objectRef), m_callback(callback)
	{
	}
}
