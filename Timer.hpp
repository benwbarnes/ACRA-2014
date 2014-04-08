#ifndef TIMER_H
#define TIMER_H

#include <opencv.hpp>

class Timer
{
	private:
		const char *_name;
		double _prevTicks;
	public:
		Timer(const char*);
		void mark(); /* Tells the timer to start counting. */
		double read(); /* Returns time in seconds since mark() was last called. */
		void print();
};

#endif
