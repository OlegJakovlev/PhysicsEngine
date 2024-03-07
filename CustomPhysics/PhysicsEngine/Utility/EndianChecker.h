#pragma once

#ifndef EndianChecker_H
#define EndianChecker_H

class EndianChecker
{
public:
    static bool IsBigEndian()
    {
        int i = 1;
        return !*((char*)&i);
    }

    static void SwapBytes(char* buffer, size_t size)
    {
        for (size_t i = 0; i < size / 2; ++i)
        {
            char temp = buffer[i];
            buffer[i] = buffer[size - 1 - i];
            buffer[size - 1 - i] = temp;
        }
    }
};

#endif