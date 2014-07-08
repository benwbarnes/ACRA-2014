#ifndef POINT_MANAGER_H
#define POINT_MANAGER_H

#include <opencv.hpp>
#include "FlowFrame.hpp"

class PointManager
{
public:
	PointManager(libconfig::Config &config):
		config(config),
		maxPoints(0),
		minPoints(0)
	{
		maxPoints = config.lookup("tracker.maxPoints");
		minPoints = config.lookup("tracker.minPoints");
		if(maxPoints < minPoints)
		{
			throw std::runtime_error(std::string("tracker/minPoints must be less than or equal to tracker/maxPoints"));
		}
	}

	void initialise(FlowFrame &frame)
	{
		cv::goodFeaturesToTrack(frame.image, frame.points, maxPoints, 0.01, 10, cv::Mat(), 3, 0, 0.04);
	}

	void update(FlowFrame &frame)
	{
		auto pt = frame.points.begin();
		auto st = frame.status.begin();
		auto er = frame.error.begin();

		while(pt != frame.points.end())
		{
			if(*st == 0)
			{
				pt = frame.points.erase(pt);
				st = frame.status.erase(st);
				er = frame.error.erase(er);
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

private:
	libconfig::Config &config;
	int maxPoints;
	int minPoints;
};

#endif
