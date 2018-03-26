#pragma once

#include <Windows.h>
#include <iostream>
#include "process.h"

class timer{

private:
	LARGE_INTEGER performanceCountStart, performanceCountEnd;
	__int64 freq;
	double time;

	LARGE_INTEGER startTimer();
	LARGE_INTEGER endTimer();

public:
	void start();
	double end();
	void showTime() const;
	timer();
	~timer();
};

