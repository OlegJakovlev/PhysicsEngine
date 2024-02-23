#pragma once

#ifndef DemoRecorder_H
#define DemoRecorder_H

#include "../Engine/EventTracker/EventTypes/PhysxEvent.h"
#include "../GlobalDefine.h"
#include <fstream>

namespace PhysicsEngine
{
    class DemoRecorder
    {
    protected:
        std::ofstream m_demoFile;

#ifdef PHYSICS_DEBUG_MODE
        std::ofstream m_debugDemoFile;
#endif

    public:
        class DemoHeader
        {

        };

        class DemoBody
        {

        };

        bool Init(uint32_t sceneId);
        void Record(PhysxEvent* eventToRecord);
        void StopRecord();
    };
}

#endif