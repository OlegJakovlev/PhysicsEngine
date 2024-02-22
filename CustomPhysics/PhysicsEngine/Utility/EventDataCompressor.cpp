#include "EventDataCompressor.h"
#include <corecrt_malloc.h>
#include "lz4.h"
#include <iostream>

namespace PhysicsEngine
{
	// https://github.com/lz4/lz4/blob/dev/examples/simple_buffer.c
	char* EventDataCompressor::Compress(const char* const dataToCompress, int dataSize)
	{
		// LZ4 provides a function that will tell you the maximum size of compressed output based on input data via LZ4_compressBound().
		const int max_dst_size = LZ4_compressBound(dataSize);
		char* compressed_data = (char*)malloc((size_t)max_dst_size);

		if (compressed_data == NULL)
		{
			std::printf("Failed to allocate memory for *compressed_data!");
			return nullptr;
		}

		// That's all the information and preparation LZ4 needs to compress *src into* compressed_data.
		// Invoke LZ4_compress_default now with our size values and pointers to our memory locations.
		// Save the return value for error checking.
		const int compressed_data_size = LZ4_compress_default(dataToCompress, compressed_data, dataSize, max_dst_size);

		// Check return_value to determine what happened.
		if (compressed_data_size <= 0)
		{
			std::printf("A 0 or negative result from LZ4_compress_default() indicates a failure trying to compress the data. ");
			return nullptr;
		}

		if (compressed_data_size > 0)
		{
			std::printf("Successfully compressed some data! Ratio: %.2f\n", (float)compressed_data_size / dataSize);
		}

		// Not only does a positive return_value mean success, the value returned == the number of bytes required.
		// You can use this to realloc() *compress_data to free up memory, if desired.  We'll do so just to demonstrate the concept.
		compressed_data = (char*) realloc(compressed_data, (size_t) compressed_data_size);

		if (compressed_data == NULL)
		{
			std::printf("Failed to re-alloc memory for compressed_data.  Sad :(");
			return nullptr;
		}

		return compressed_data;
	}

	char* EventDataCompressor::Decompress(char* compressedData, int compressedDataSize, int expectedDataSize)
	{
		// First, let's create a *new_src location of size src_size since we know that value.
		char* const regen_buffer = (char*)malloc(expectedDataSize);

		if (regen_buffer == NULL)
		{
			std::printf("Failed to allocate memory for *regen_buffer.");
			return nullptr;
		}

		// The LZ4_decompress_safe function needs to know where the compressed data is, how many bytes long it is,
		// where the regen_buffer memory location is, and how large regen_buffer (uncompressed) output will be.
		// Again, save the return_value.
		const int decompressed_size = LZ4_decompress_safe(compressedData, regen_buffer, compressedDataSize, expectedDataSize);

		// No longer needed
		free(compressedData);

		if (decompressed_size < 0)
		{
			std::printf("A negative result from LZ4_decompress_safe indicates a failure trying to decompress the data.  See exit code (echo $(%i)) for value returned.", decompressed_size);
			return nullptr;
		}

		if (decompressed_size >= 0)
		{
			std::printf("We successfully decompressed some data!\n");
		}

		return regen_buffer;
	}
}
