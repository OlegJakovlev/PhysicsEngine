#pragma once

#ifndef InputLogger_H
#define InputLogger_H

#include "Logger.h"

namespace CustomApplication
{
	class InputLogger : public Logger
	{
		static InputLogger* GetInstance();
	};
}


#endif
