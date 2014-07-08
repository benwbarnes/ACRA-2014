#ifndef DISPLAY_H
#define DISPLAY_H

#include <libconfig.h++>
#include <opencv.hpp>
#include "FlowFrame.hpp"

class Display
{
public:
	Display(std::string, libconfig::Config&);
	Display(const Display&) = delete;
	Display& operator= (const Display&) = delete;
	~Display();
	char render(const cv::Mat&);
	char render(const FlowFrame&);

private:
	const std::string appTitle;
	void imageToBGR(cv::Mat &image);
	libconfig::Config &config;
	bool displayOn;
	int frameCount;
	cv::Mat workingFrame;
};

#endif
