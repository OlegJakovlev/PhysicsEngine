#pragma once

#ifndef Logger_H
#define Logger_H

#include "spdlog.h"

namespace PhysicsEngine
{
	class Logger
	{
	protected:
		std::shared_ptr<spdlog::logger> logger;
	};
}

#endif