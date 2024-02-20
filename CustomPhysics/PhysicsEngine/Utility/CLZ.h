#pragma once

#ifndef CLZ_H
#define CLZ_H

#include <cstdint>

class CLZ
{
private:
    static uint8_t m_clzLookup[256];

public:
    static uint32_t CLZ1(uint32_t x);
};

#endif