#pragma once

#ifdef CustomPhysics
#define PHYSICSDLL_API __declspec(dllexport)
#else
#define PHYSICSDLL_API __declspec(dllimport)
#endif

// Print outs, logs
#define PHYSICS_DEBUG_MODE

// Do we want to have remote visual debug (PhysX Visual Debugger)
//#define REMOTE_VISUAL_DEBUG