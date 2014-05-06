#include "Tracker.hpp"

Tracker::Tracker(libconfig::Config &config, TrackingAlgorithm &algorithm) :	config(config),
																			fly(config),
																			algorithm(algorithm),
																			termCrit(cv::TermCriteria(cv::TermCriteria::COUNT+cv::TermCriteria::EPS, 30, 0.01)),
																			frames(),
																			queueMutex(),
																			maxPoints(0)
{
	maxPoints = config.lookup("tracker.maxPoints");
}

void Tracker::update()
{
	/* If no frames have yet been buffered, then the next (first) frame undergoes point generation. */ 
	while(frames.size() < 1) {
		FlowFrame nextFrame(fly.nextFrame());
		cv::goodFeaturesToTrack(nextFrame.image, nextFrame.points, maxPoints, 0.01, 10, cv::Mat(), 3, 0, 0.04);
		frames.push(nextFrame);
	}

	/* Two frames are needed for optical flow. */
	while(frames.size() < 2) {
		FlowFrame nextFrame(fly.nextFrame());
		frames.push(nextFrame);
	}

	std::lock_guard<std::mutex> locker(queueMutex);
	FlowFrame first = frames.front();
	frames.pop();
	algorithm.track(first, frames.front());
}

FlowFrame Tracker::getFrame()
{
	std::lock_guard<std::mutex> locker(queueMutex);
	FlowFrame &frame = frames.front();
	return frame;
}
