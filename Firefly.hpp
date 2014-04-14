#ifndef FIREFLY_H
#define FIREFLY_H

#include <FlyCapture2.h>
#include <opencv.hpp>

using namespace FlyCapture2;
#define EXTERNAL 0
#define FREERUN 1

class Firefly
{
	private:
	Camera cam; /* Point Grey camera object. */
	PGRGuid guid; /* Identifier for the camera being used. */
	CameraInfo camInfo;
	BusManager busMgr; /* Point Grey BusManager, enumerates cameras. */
	Error error; /* Point Grey error container. */
	cv::Mat image;
	int seqNum; /* Sequence is zero-indexed; -1 indicates no images captured. */

	void PrintError(Error);

	public:
	Firefly();
	int open();
	int close();
	int start(int);
	void pushImage(Image*);
	int getSeqNum();
	cv::Mat getImage();
};

#endif
