#include "GraphicsLogger.h"

namespace CustomApplication
{
	GraphicsLogger* GraphicsLogger::GetInstance()
	{
		static GraphicsLogger instance;
		return &instance;
	}
}