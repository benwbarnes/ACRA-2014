#ifndef DISPLAY_H
#define DISPLAY_H

#include <opencv.hpp>
#include "FlowFrame.hpp"

class Display
{
	public:
		Display(std::string);
		Display(const Display&) = delete;
		Display& operator= (const Display&) = delete;
		~Display();
		char render(const cv::Mat&);
		char render(const FlowFrame&);

	private:
		const std::string appTitle;
		void imageToBGR(cv::Mat &image);
};

#endif
