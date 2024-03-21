#include "MouseCallbackEntry.h"

namespace CustomApplication
{
	MouseMoveCallbackEntry::MouseMoveCallbackEntry(void* objectRef, MouseMoveCallback callback) : m_objectRef(objectRef), m_callback(callback)
	{
	}
}