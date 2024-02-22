#include "DemoRecorder.h"
#include <cassert>
#include "../Utility/EventDataCompressor.h"
#include "../Utility/EndianChecker.h"
#include "../GlobalDefine.h"
#include <string>

namespace PhysicsEngine
{
    bool DemoRecorder::Init(uint32_t sceneId)
    {
#ifdef isBigEndian
        assert(EndianChecker::IsBigEndian());
#else
        assert(EndianChecker::!IsBigEndian());
#endif

        std::string fileName = "Demo_" + std::to_string(sceneId);
        m_demoFile.open(fileName, std::ios::binary);

        if (!m_demoFile.is_open())
        {
            return false;
        }

#ifdef PHYSICS_DEBUG_MODE
        std::string debugFileName = "Debug_" + std::to_string(sceneId);
        m_debugDemoFile.open(debugFileName, std::ios::in);

        if (!m_debugDemoFile.is_open())
        {
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

        m_demoFile << eventToRecord->GetType();
        m_demoFile << byteSize;
        m_demoFile << EventDataCompressor::Compress(buffer, byteSize);

#ifdef PHYSICS_DEBUG_MODE
        m_debugDemoFile << buffer;
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
