#include "Timer.h"

double Timer::GetCurrentTime() const
{
	return std::chrono::duration<double>(
		std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::high_resolution_clock::now().time_since_epoch()))
		.count();
}

double Timer::GetElapsedTime() const
{
	return std::chrono::duration<double>(std::chrono::duration_cast<std::chrono::milliseconds>(stopTime - startTime)).count();
}

void Timer::Reset()
{
	startTime = std::chrono::high_resolution_clock::now();
}

void Timer::Stop()
{
	stopTime = std::chrono::high_resolution_clock::now();
}