#include "SystemClock.h"

std::chrono::high_resolution_clock SystemClock::clock;

uint64_t SystemClock::milli_secs()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(clock.now().time_since_epoch()).count();
}

uint64_t SystemClock::micro_secs()
{
	return std::chrono::duration_cast<std::chrono::microseconds>(clock.now().time_since_epoch()).count();
}

uint64_t SystemClock::nano_secs()
{
	return std::chrono::duration_cast<std::chrono::nanoseconds>(clock.now().time_since_epoch()).count();
}
