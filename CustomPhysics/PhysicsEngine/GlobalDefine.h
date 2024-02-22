#pragma once

#ifdef CustomPhysics
#define PHYSICSDLL_API __declspec(dllexport)
#else
#define PHYSICSDLL_API __declspec(dllimport)
#endif

#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#define isBigEndian
#else
#define isSmallEndian
#endif

// Print outs, logs
#define PHYSICS_DEBUG_MODE

// Do we want to have remote visual debug (PhysX Visual Debugger)
#define REMOTE_VISUAL_DEBUG