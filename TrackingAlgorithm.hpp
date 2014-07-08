#ifndef TRACKING_ALGORITHM
#define TRACKING_ALGORITHM

#include <opencv.hpp>
#include "FlowFrame.hpp"

class TrackingAlgorithm
{
public:
	virtual ~TrackingAlgorithm() {};
	virtual int track(FlowFrame&, FlowFrame&) = 0;
};

class KLT : public TrackingAlgorithm
{
public:
	KLT() : termCrit(cv::TermCriteria(cv::TermCriteria::COUNT+cv::TermCriteria::EPS, 30, 0.01)) {};
	int track(FlowFrame&, FlowFrame&);
private:
	cv::TermCriteria termCrit;
};

#endif