#ifndef FIREFLY_H
#define FIREFLY_H

#include <condition_variable>
#include <flycapture/FlyCapture2.h>
#include <libconfig.h++>
#include <opencv2/opencv.hpp>
#include <map>
#include <mutex>
#include <queue>
#include <stdexcept>

class Firefly
{
	public:
		Firefly(libconfig::Config&);
		Firefly(const Firefly&) = delete;
		Firefly& operator= (const Firefly&) = delete;
		~Firefly();
		void pushImage(FlyCapture2::Image*); /* Used as part of callback */
		cv::Mat nextFrame();

	private:
		libconfig::Config &config;

		/* PointGrey resources */
		FlyCapture2::Camera cam;
		FlyCapture2::BusManager busMgr; /* Used initially to find a camera. */
		FlyCapture2::PGRGuid guid; /* Identifier for the camera represented by this object */
		FlyCapture2::Error error;

		/* Other member resources */
		std::queue<cv::Mat> bufferQueue;
		std::mutex queueMutex;
		std::condition_variable dataReady;
		std::map<std::string, FlyCapture2::FrameRate> framerates;

		/* Wrappers for basic camera operations */
		int open();
		int close();
		int start();
		int stop();
};

#endif
