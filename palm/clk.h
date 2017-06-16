#ifndef MY_CLOCK_H
#define MY_CLOCK_H
#pragma once
#include <windows.h>
#include <stdio.h>

class my_clock {
private:
	LARGE_INTEGER	freq;
	LARGE_INTEGER	start_t, stop_t;
public:
	my_clock();
	~my_clock();
	void	start();
	double	stop();
};

#endif