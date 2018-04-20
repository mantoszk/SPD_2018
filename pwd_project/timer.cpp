#include "timer.h"

LARGE_INTEGER timer::startTimer(){

	LARGE_INTEGER start;
	DWORD_PTR oldmask = SetThreadAffinityMask(GetCurrentThread(), 0);
	QueryPerformanceCounter(&start);
	SetThreadAffinityMask(GetCurrentThread(), oldmask);
	return start;
}

LARGE_INTEGER timer::endTimer(){

	LARGE_INTEGER stop;
	DWORD_PTR oldmask = SetThreadAffinityMask(GetCurrentThread(), 0);
	QueryPerformanceCounter(&stop);
	SetThreadAffinityMask(GetCurrentThread(), oldmask);
	return stop;
}

void timer::start(){
	time = 0.0;
	performanceCountStart = startTimer();
}

double timer::end(){

	performanceCountEnd = endTimer();
	time = (double)(performanceCountEnd.QuadPart - performanceCountStart.QuadPart) / freq;
	return time;
}

void timer::showTime() const{

	std::cout << "Elapsed time: " << time << "s" << std::endl;
}

timer::timer(){

	QueryPerformanceFrequency((LARGE_INTEGER *)&freq);
	time = 0.0;
}

timer::~timer(){

}
