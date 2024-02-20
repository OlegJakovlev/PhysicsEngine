#pragma once

#ifndef DoubleBufferAllocator_H
#define DoubleBufferAllocator_H

namespace PhysicsEngine
{
	class DoubleBufferAllocator
	{
	public:
		void SwapBuffers();
		void ClearCurrentBuffer();
	};
}

#endif