#pragma once

#ifndef DemoRecorder_H
#define DemoRecorder_H

#include "../Engine/EventTracker/EventTypes/PhysxEvent.h"

namespace PhysicsEngine
{
    class DemoRecorder
    {
    public:
        class DemoHeader
        {

        };

        class DemoBody
        {

        };

        bool Init();
        void Record(PhysxEvent* eventToRecord);

        //void Binarize(DemoEntry* entry);
        //void Debinarize();
    };
}

#endif