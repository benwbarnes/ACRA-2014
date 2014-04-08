#ifndef DISPLAY_H
#define DISPLAY_H

#include <opencv.hpp>
#include "Points.hpp"

class Display
{
	private:
		const char *_appTitle;
		void addPoints(cv::Mat, Points);

	public:
		Display(const char*);
		~Display();
		char render(cv::Mat, Points);
};

#endif
