#include "ApplicationLogger.h"

namespace CustomApplication
{
	ApplicationLogger* ApplicationLogger::GetInstance()
	{
		static ApplicationLogger instance;
		return &instance;
	}
}

