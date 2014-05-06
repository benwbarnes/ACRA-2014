#ifndef FLOW_FRAME_H
#define FLOW_FRAME_H

#include <opencv.hpp>

class FlowFrame
{
	public:
		FlowFrame(cv::Mat image) : image(image), points(), status(), error() {}
		/* Member-wise copy is correct, no explicit copy constructor required. */
		cv::Mat image;
		std::vector<cv::Point2f> points;
		std::vector<uint8_t> status;
		std::vector<float> error;
};

#endif