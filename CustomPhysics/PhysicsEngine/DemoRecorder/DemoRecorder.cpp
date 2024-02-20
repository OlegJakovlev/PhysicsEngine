#include "DemoRecorder.h"

bool DemoRecorder::Init()
{
    return true;
}

void DemoRecorder::Record(PhysxEvent* eventToRecord)
{
    eventToRecord->Serialize();
}
