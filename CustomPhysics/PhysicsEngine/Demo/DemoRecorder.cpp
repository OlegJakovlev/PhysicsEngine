﻿#include "DemoRecorder.h"
#include <cassert>
#include "../Utility/EventDataCompressor.h"
#include "../Utility/EndianChecker.h"
#include "../GlobalDefine.h"
#include <string>

namespace PhysicsEngine
{
    bool DemoRecorder::Init(uint32_t sceneId)
    {

        // TODO: I have no idea why runtime check fails
#ifdef isBigEndian
        //assert(EndianChecker::IsBigEndian());
#else
        //assert(!EndianChecker::IsBigEndian());
#endif

        std::string fileName = "./Demos/Demo_" + std::to_string(sceneId) + ".dem";
        m_demoFile.open(fileName, std::ios::out);

        if (!m_demoFile.is_open())
        {
            std::printf("Demo file can not be opened for writing!\n");
            return false;
        }

#ifdef PHYSICS_DEBUG_MODE
        std::string debugFileName = "./Demos/Debug_" + std::to_string(sceneId) + ".dem";
        m_debugDemoFile.open(debugFileName, std::ios::out);

        if (!m_debugDemoFile.is_open())
        {
            std::printf("Debug demo file can not be opened for writing!\n");
            return false;
        }
#endif

        return true;
    }

    void DemoRecorder::Record(PhysxEvent* eventToRecord)
    {
        uint32_t byteSize = eventToRecord->GetSize();

        char* buffer = new char[byteSize];
        eventToRecord->Serialize(buffer);

        m_demoFile << byteSize;
        m_demoFile << EventDataCompressor::Compress(buffer, byteSize);

#ifdef PHYSICS_DEBUG_MODE
        m_debugDemoFile << eventToRecord->SerializeDebug();
#endif
    }

    void DemoRecorder::StopRecord()
    {
        if (m_demoFile.is_open())
        {
            m_demoFile.close();
        }

#ifdef PHYSICS_DEBUG_MODE
        if (m_debugDemoFile.is_open())
        {
            m_debugDemoFile.close();
        }
#endif
    }
}
