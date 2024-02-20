#include "DemoLogger.h"

namespace PhysicsEngine
{
    DemoLogger* DemoLogger::GetInstance()
    {
        static DemoLogger instance;
        return &instance;
    }
}