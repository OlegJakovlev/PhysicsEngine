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
};

#endif