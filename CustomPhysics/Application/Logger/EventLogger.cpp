#include "EventLogger.h"

namespace CustomApplication
{
	EventLogger* EventLogger::GetInstance()
	{
		static EventLogger instance;
		return &instance;
	}
}