#pragma once

#ifndef DemoLogger_H
#define DemoLogger_H

#include "Logger.h"

namespace PhysicsEngine
{
    class DemoLogger : public Logger
    {
        static DemoLogger* GetInstance();
    };
}

#endif
