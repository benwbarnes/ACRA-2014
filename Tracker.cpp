#include "Tracker.hpp"

Tracker::Tracker(libconfig::Config &config, TrackingAlgorithm &algorithm, Camera &cam):
	config(config),
	cam(cam),
	algorithm(algorithm),
	termCrit(cv::TermCriteria(cv::TermCriteria::COUNT+cv::TermCriteria::EPS, 30, 0.01)),
	frames(),
	queueMutex(),
	maxPoints(0),
	pMgr(config)
{
	maxPoints = config.lookup("tracker.maxPoints");
}

void Tracker::update()
{
	/* If no frames have yet been buffered, then the next (first) frame undergoes point generation. */ 
	while(frames.size() < 1) {
		FlowFrame nextFrame(cam.nextFrame());
		pMgr.initialise(nextFrame);
		frames.push(nextFrame);
	}

	/* Two frames are needed for optical flow. */
	while(frames.size() < 2) {
		FlowFrame nextFrame(cam.nextFrame());
		frames.push(nextFrame);
	}

	std::lock_guard<std::mutex> locker(queueMutex);
	FlowFrame first = frames.front();
	frames.pop();
	algorithm.track(first, frames.front());
	pMgr.update(frames.front());
}

FlowFrame Tracker::getFrame()
{
	std::lock_guard<std::mutex> locker(queueMutex);
	FlowFrame &frame = frames.front();
	return frame;
}
