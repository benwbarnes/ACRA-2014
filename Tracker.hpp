#ifndef TRACKER_H
#define TRACKER_H

#include <libconfig.h++>
#include <opencv.hpp>
#include <mutex>
#include <queue>
#include "Firefly.hpp"
#include "FlowFrame.hpp"
#include "TrackingAlgorithm.hpp"

class Tracker
{
	public:
		Tracker(libconfig::Config&, TrackingAlgorithm&);
		void update();
		FlowFrame getFrame();
	private:
		libconfig::Config &config;
		Firefly fly;
		TrackingAlgorithm &algorithm;
		cv::TermCriteria termCrit;
		std::queue<FlowFrame> frames;
		std::mutex queueMutex;
		int maxPoints;
};
#endif