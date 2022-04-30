// https://stackoverflow.com/a/24045970/16610064
#ifdef linux

	#include <time.h>
	int milliDiff(struct timespec *start, struct timespec *end) {
	    return ((end->tv_sec * 1000) + (end->tv_nsec / 1000000)) -
	        ((start->tv_sec * 1000) + (start->tv_nsec / 1000000));
	}
	typedef struct timespec Time;
	#define getTime(t) clock_gettime(CLOCK_MONOTONIC, t);

#elif _WIN32

	#include <Windows.h>
	int milliDiff(LARGE_INTEGER *start, LARGE_INTEGER *end)
	{
	    LARGE_INTEGER Frequency, elapsed;
	
	    QueryPerformanceFrequency(&Frequency); 
	    elapsed.QuadPart = end->QuadPart - start->QuadPart;
	
	    elapsed.QuadPart *= 1000;
	    elapsed.QuadPart /= Frequency.QuadPart;
	
	    return elapsed.QuadPart;
	}
	typedef LARGE_INTEGER Time;
	#define getTime(t) QueryPerformanceCounter(t);

#else
	#error OS not supported, repl-src/timer.h
#endif
