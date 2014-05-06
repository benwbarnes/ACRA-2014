#include "Display.hpp"

Display::Display(std::string appTitle) : appTitle(appTitle)
{
	cv::namedWindow(appTitle, cv::WINDOW_AUTOSIZE);
}

Display::~Display()
{
	cv::destroyWindow(appTitle);
}

char Display::render(const cv::Mat &frame)
{
	char keyPressed;
	cv::imshow(appTitle, frame);
	keyPressed = cv::waitKey(1);
	return keyPressed;
}

char Display::render(const FlowFrame &frame)
{
	cv::Mat image = frame.image.clone();

	if(frame.status.size() != frame.points.size()) {
		std::cerr << "Frame error: \"points\" and \"status\" vectors of different dimension." << std::endl;
		return '\0';
	}

	imageToBGR(image); /* Allows coloured dots to be displayed. */
	for(uint i = 0; i < frame.status.size(); i++) {
		if(frame.status[i] != 0) {
			cv::circle(image, frame.points[i], 2, cv::Scalar(0, 255, 0), -1, 8);
		}
	}

	char keyPressed = render(image);
	return keyPressed;
}

void Display::imageToBGR(cv::Mat &image)
{
	std::vector<cv::Mat> channels;
	for(int i = 0; i < 3; i++) channels.push_back(image);
	cv::merge(channels, image);
}