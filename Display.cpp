#include "Display.hpp"

Display::Display(std::string appTitle, libconfig::Config &config) : appTitle(appTitle), config(config), displayOn(false), frameCount(0), workingFrame()
{
	displayOn = config.lookup("display.displayOn");
	if(displayOn) {
		cv::namedWindow(appTitle, cv::WINDOW_AUTOSIZE);
	}
}

Display::~Display()
{
	if(displayOn) {
		cv::destroyWindow(appTitle);
	}
}

char Display::render(const cv::Mat &frame)
{
	char keyPressed = '\0';
	if(displayOn) {
		cv::imshow(appTitle, frame);
		keyPressed = cv::waitKey(1);
	}
	else {
		std::cout << "Frame: " << frameCount << std::endl;
	}
	frameCount++;
	return keyPressed;
}

char Display::render(const FlowFrame &frame)
{
	char keyPressed = '\0';
	if(displayOn) {
		workingFrame = frame.image.clone();

		if(frame.status.size() != frame.points.size()) {
			std::cerr << "Frame error: \"points\" and \"status\" vectors of different dimension." << std::endl;
			return '\0';
		}

		imageToBGR(workingFrame); /* Allows coloured dots to be displayed. */
		for(uint i = 0; i < frame.status.size(); i++) {
			if(frame.status[i] != 0) {
				cv::circle(workingFrame, frame.points[i], 2, cv::Scalar(0, 255, 0), -1, 8);
			}
		}
	}
	keyPressed = render(workingFrame);
	return keyPressed;
}

void Display::imageToBGR(cv::Mat &image)
{
	std::vector<cv::Mat> channels;
	for(int i = 0; i < 3; i++) channels.push_back(image);
	cv::merge(channels, image);
}
