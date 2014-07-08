#include "PointManager.hpp"

PointManager::PointManager(libconfig::Config &config):
	config(config),
	maxPoints(0),
	minPoints(0)
{
	maxPoints = config.lookup("tracker.maxPoints");
	minPoints = config.lookup("tracker.minPoints");
	if(maxPoints < minPoints)
	{
		throw std::runtime_error(std::string("tracker/minpoints must be less than or equal to tracker/maxPoints"));
	}
}

void PointManager::initialise(FlowFrame &frame)
{
	cv::goodFeaturesToTrack(frame.image, frame.points, maxPoints, 0.01, 10, cv::Mat(), 3, 0, 0.04);
}

void PointManager::update(FlowFrame &frame)
{
	auto pt = frame.points.begin();
	auto st = frame.status.begin();
	auto er = frame.error.begin();

	while(pt != frame.points.end())
	{
		if(*st == 0)
		{
//			pt = frame.points.erase(pt);
//			st = frame.status.erase(pt);
//			er = frame.error.erase(pt);
		}

		++pt;
		++st;
		++er;
	}

	if((int)frame.points.size() < minPoints)
	{
		initialise(frame);
	}
}
