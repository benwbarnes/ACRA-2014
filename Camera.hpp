#ifndef CAMERA_H
#define CAMERA_H

class Camera
{
public:
	virtual ~Camera() {};
	virtual cv::Mat nextFrame() = 0;
};

#endif