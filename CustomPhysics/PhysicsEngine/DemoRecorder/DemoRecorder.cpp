#include "DemoRecorder.h"

namespace PhysicsEngine
{
    bool DemoRecorder::Init()
    {
        return true;
    }

    void DemoRecorder::Record(PhysxEvent* eventToRecord)
    {
        eventToRecord->Serialize();
    }
}
