#ifndef POINTS_H
#define POINTS_H

#include <vector>
#include <opencv.hpp>

typedef struct
{
	std::vector<cv::Point2f> points;
	std::vector<uint8_t> status;
	std::vector<float> error;
} Points;

#endif
