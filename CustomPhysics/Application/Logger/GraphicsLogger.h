#pragma once

#ifndef GraphicsLogger_H
#define GraphicsLogger_H

#include "Logger.h"

namespace CustomApplication
{
	class GraphicsLogger : public Logger
	{
		GraphicsLogger* GraphicsLogger::GetInstance();
	};
}

#endif