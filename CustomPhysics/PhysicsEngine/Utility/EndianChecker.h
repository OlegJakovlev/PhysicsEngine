#pragma once

#ifndef EndianChecker_H
#define EndianChecker_H

class EndianChecker
{
public:
    static inline int IsBigEndian()
    {
        int i = 1;
        return !*((char*)&i);
    }
};

#endif