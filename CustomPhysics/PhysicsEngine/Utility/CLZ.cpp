#include "CLZ.h"

uint8_t CLZ::m_clzLookup[256] =
{
    32U, 31U, 30U, 30U, 29U, 29U, 29U, 29U,
    28U, 28U, 28U, 28U, 28U, 28U, 28U, 28U,
    27U, 27U, 27U, 27U, 27U, 27U, 27U, 27U,
    27U, 27U, 27U, 27U, 27U, 27U, 27U, 27U,
    26U, 26U, 26U, 26U, 26U, 26U, 26U, 26U,
    26U, 26U, 26U, 26U, 26U, 26U, 26U, 26U,
    26U, 26U, 26U, 26U, 26U, 26U, 26U, 26U,
    26U, 26U, 26U, 26U, 26U, 26U, 26U, 26U,
    25U, 25U, 25U, 25U, 25U, 25U, 25U, 25U,
    25U, 25U, 25U, 25U, 25U, 25U, 25U, 25U,
    25U, 25U, 25U, 25U, 25U, 25U, 25U, 25U,
    25U, 25U, 25U, 25U, 25U, 25U, 25U, 25U,
    25U, 25U, 25U, 25U, 25U, 25U, 25U, 25U,
    25U, 25U, 25U, 25U, 25U, 25U, 25U, 25U,
    25U, 25U, 25U, 25U, 25U, 25U, 25U, 25U,
    25U, 25U, 25U, 25U, 25U, 25U, 25U, 25U,
    24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
    24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
    24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
    24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
    24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
    24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
    24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
    24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
    24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
    24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
    24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
    24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
    24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
    24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
    24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
    24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U
};

// https://github.com/Ooken/NekoEngine/blob/master/Tree/Example.cpp#L308
// http://embeddedgurus.com/state-space/2014/09/fast-deterministic-and-portable-counting-leading-zeros/
uint32_t CLZ::CLZ1(uint32_t x)
{
    uint32_t n;

    if (x >= (1U << 16))
    {
        if (x >= (1U << 24))
        {
            n = 24U;
        }
        else
        {
            n = 16U;
        }
    }
    else
    {
        if (x >= (1U << 8))
        {
            n = 8U;
        }
        else
        {
            n = 0U;
        }
    }

    return (uint32_t) m_clzLookup[x >> n] - n;
}