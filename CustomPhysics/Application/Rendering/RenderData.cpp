#include "RenderData.h"
#include "../GlutApp.h"
#include "../Utility/Hashing.h"

namespace CustomApplication
{
    RenderData::RenderData()
    {
        auto defaultColorEntry = GlutApp::Get()->GetColorDatabase()->GetEntry(CRC32_STR("Default"));
        m_color = defaultColorEntry.data;
        m_disableLighting = false;
        m_lineWidth = 1;
    }

    void RenderData::SetColour(const physx::PxVec3* newColor)
    {
        m_color = newColor;
    }
    
    physx::PxVec3 const* RenderData::GetColor() const
    {
        return m_color;
    }

    bool RenderData::GetIsLightingDisabled() const
    {
        return m_disableLighting;
    }

    void RenderData::SetLightingDisabled(bool isDisabled)
    {
        m_disableLighting = isDisabled;
    }

    float RenderData::GetLineWidth() const
    {
        return m_lineWidth;
    }

    void RenderData::SetLineWidth(float width)
    {
        m_lineWidth = width;
    }
}