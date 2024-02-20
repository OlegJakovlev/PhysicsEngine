#include "InputLogger.h"

namespace CustomApplication
{
	InputLogger* InputLogger::GetInstance()
	{
		static InputLogger instance;
		return &instance;
	}
}