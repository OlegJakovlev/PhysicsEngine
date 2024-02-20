#pragma once

#ifndef DoubleBufferAllocator_H
#define DoubleBufferAllocator_H

class DoubleBufferAllocator
{
public:
	void SwapBuffers();
	void ClearCurrentBuffer();
};

#endif