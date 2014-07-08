#ifndef CVVIDEOCAPTURE_H
#define CVVIDEOCAPTURE_H

#include <opencv.hpp>
#include "Camera.hpp"

class CVVideoCapture : public Camera
{
public:
	CVVideoCapture(): 
		cap(0)
	{
		if(!cap.isOpened())
		{
			throw std::runtime_error(std::string("Error opening native camera."));
		}
		cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
		cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
	}
	
	cv::Mat nextFrame()
	{
		cv::Mat frame;
		cap >> frame;
		cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);
		cv::flip(frame, frame, 1);
		return frame;
	}

private:
	cv::VideoCapture cap;
};

#endif