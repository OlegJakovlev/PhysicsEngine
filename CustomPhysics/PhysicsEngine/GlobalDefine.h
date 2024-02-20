#pragma once

#ifdef CustomPhysics
#define PHYSICSDLL_API __declspec(dllexport)
#else
#define PHYSICSDLL_API __declspec(dllimport)
#endif

// Supports input
#define DEBUG_INPUT

// Do we want to have any kind of window
#define DEBUG_RENDERER

// Print outs, logs
#define DEBUG_MODE

// Do we want to have remote visual debug (PhysX Visual Debugger)
#define REMOTE_VISUAL_DEBUG

// Allow demo recording
#define DEMO_RECORD