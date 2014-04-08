#ifndef TRACKER_H
#define TRACKER_H

#include <opencv.hpp>
#include "Points.hpp"

class Tracker
{
	private:
		cv::TermCriteria _termCrit;
		int _maxPoints;
	public:
		Tracker(cv::TermCriteria);
		std::vector<cv::Point2f> getPoints(cv::Mat);
		Points calcFlow(cv::Mat, cv::Mat, Points);
		void setMaxPoints(int);
};

#endif
