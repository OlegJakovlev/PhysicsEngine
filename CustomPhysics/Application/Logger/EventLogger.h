#pragma once

#ifndef EventLogger_H
#define EventLogger_H

#include "Logger.h"

namespace CustomApplication
{
	// High-level application events, such as startup, shutdown, initialization, and configuration
	class EventLogger : public Logger
	{
		EventLogger* EventLogger::GetInstance();
	};
}

#endif