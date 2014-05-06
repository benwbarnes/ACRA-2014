#include "TrackingAlgorithm.hpp"

int KLT::track(FlowFrame &first, FlowFrame &second)
{
	cv::calcOpticalFlowPyrLK(first.image, second.image, first.points, second.points, second.status, second.error, cv::Size(31, 31), 3, termCrit, 0, 0.001);
	return 0;
}