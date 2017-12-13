#include "System.h"
#include <chrono>
#include <ctime>

int64_t System::microtime()
{
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	return (ts.tv_sec * 1000000 + ts.tv_nsec / 1000);
}

int64_t System::nanotime()
{
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
	return (ts.tv_sec * 1000000000 + ts.tv_nsec);
}
