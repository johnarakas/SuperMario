#include "SystemTime.h"

uint64_t SystemTime::currentTime = 0;

uint64_t SystemTime::GetCurrentTime()
{
	return currentTime;
}

void SystemTime::SetCurrentTime()
{
	currentTime = GETTIME;
}
