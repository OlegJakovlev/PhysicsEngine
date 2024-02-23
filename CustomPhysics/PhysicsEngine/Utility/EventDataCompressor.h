#pragma once

#ifndef EventDataCompressor_H
#define EventDataCompressor_H

namespace PhysicsEngine
{
	class EventDataCompressor
	{
	public:
		static char* Compress(const char* const dataToCompress, int dataSize);
		static char* Decompress(char* compressedData, int compressedDataSize, int expectedDataSize);
	};
}

#endif