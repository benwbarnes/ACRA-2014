#ifndef POINT_MANAGER_H
#define POINT_MANAGER_H

#include <libconfig.h++>
#include <opencv.hpp>
#include "FlowFrame.hpp"

class PointManager
{
public:
	PointManager(libconfig::Config&);
	void initialise(FlowFrame&);
	void update(FlowFrame&);

private:
	libconfig::Config &config;
	int maxPoints;
	int minPoints;
};

#endif
