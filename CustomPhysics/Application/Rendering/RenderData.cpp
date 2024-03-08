#include "RenderData.h"
#include "../GlutApp.h"
#include "../Utility/Hashing.h"

namespace CustomApplication
{
    RenderData::RenderData()
    {
        auto defaultColorEntry = GlutApp::Get()->GetColorDatabase()->GetEntry(CRC32_STR("Default"));
        m_color = defaultColorEntry.data;
    }

    void RenderData::SetColour(const physx::PxVec3* newColor)
    {
        m_color = newColor;
    }

    physx::PxVec3 const* RenderData::GetColor() const
    {
        return m_color;
    }
}