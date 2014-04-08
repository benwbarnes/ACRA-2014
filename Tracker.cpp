#include "Tracker.hpp"

Tracker::Tracker(cv::TermCriteria termCrit) : _termCrit(termCrit), _maxPoints(100) {}

std::vector<cv::Point2f> Tracker::getPoints(cv::Mat frame)
{
	std::vector<cv::Point2f> points;
	cv::goodFeaturesToTrack(frame, points, _maxPoints, 0.01, 10, cv::Mat(), 3, 0, 0.04);

	if(points.size() == 0)
	{
		std::cout << "Error finding points." << std::endl;
	}

	cv::cornerSubPix(frame, points, cv::Size(10, 10), cv::Size(-1, -1), _termCrit);
	return points;
}

Points Tracker::calcFlow(cv::Mat firstFrame, cv::Mat secondFrame, Points firstP)
{
	Points secondP;
	cv::calcOpticalFlowPyrLK(firstFrame, secondFrame, firstP.points, secondP.points, secondP.status, secondP.error, cv::Size(31, 31), 3, _termCrit, 0, 0.001);
	return secondP;
}

void Tracker::setMaxPoints(int maxPoints)
{
	_maxPoints = maxPoints;
}
