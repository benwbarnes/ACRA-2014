#include "Display.hpp"

Display::Display(const char *appTitle) : _appTitle(appTitle)
{
	cv::namedWindow(_appTitle, cv::WINDOW_AUTOSIZE);
}

Display::~Display()
{
	cv::destroyWindow(_appTitle);
}

char Display::render(cv::Mat frame, Points p)
{
	char keyPressed;
	cv::Mat display;

	/* Converts 1-channel greyscale to 3-channel BGR */
	std::vector<cv::Mat> channels;
	for(int i = 0; i < 3; i++) channels.push_back(frame);
	cv::merge(channels, display);

	addPoints(display, p);
	cv::imshow(_appTitle, display);
	keyPressed = cv::waitKey(1);
	return keyPressed;
}

void Display::addPoints(cv::Mat frame, Points p)
{
	if(p.points.size() != p.status.size())
	{
		std::cout << "Error: points.size() != status.size()" << std::endl;
		return;
	}

	for(uint i = 0; i < p.points.size(); i++)
	{
		if(p.status[i] != 0)
		{
			cv::circle(frame, p.points[i], 2, cv::Scalar(0, 255, 0), -1, 8);
		}
	}
}
