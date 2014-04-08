#ifndef FIREFLY_H
#define FIREFLY_H

#include <FlyCapture2.h>
#include <opencv.hpp>

using namespace FlyCapture2;

class Firefly
{
	private:
	Camera cam; /* Point Grey camera object. */
	PGRGuid guid; /* Identifier for the camera being used. */
	CameraInfo camInfo;
	BusManager busMgr; /* Point Grey BusManager, enumerates cameras. */
	Error error; /* Point Grey error container. */

	void PrintError(Error);

	public:
	int open();
	int close();

	cv::Mat getCVFrame(); /* Return a Frame object with metadata. */
};

#endif
