#pragma once

#ifndef ApplicationLogger_H
#define ApplicationLogger_H

#include "Logger.h"

namespace CustomApplication
{
	class ApplicationLogger : public Logger
	{
		ApplicationLogger* ApplicationLogger::GetInstance();
	};
}

#endif