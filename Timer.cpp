#include "Timer.hpp"

Timer::Timer(const char *name) : _name(name)
{
	mark();
}

void Timer::mark()
{
	_prevTicks = (double)cv::getTickCount();
}

double Timer::read()
{
	double currentTicks = (double)cv::getTickCount();
	double result = (currentTicks - _prevTicks)/cv::getTickFrequency();
	return result;
}

void Timer::print()
{
	double period = read();
	std::cout << _name << " period:\t" << period << " seconds" << std::endl;
}
