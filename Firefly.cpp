#include "Firefly.hpp"

int Firefly::open()
{
	unsigned int numCameras;
	error = busMgr.GetNumOfCameras(&numCameras);

	if(error != PGRERROR_OK)
	{
		PrintError(error);
		return -1;
	}

	if(numCameras < 1)
	{
		printf("No cameras detected.\n");
		return -1;
	}

	printf("Number of cameras detected: %d\n", numCameras);

	error = busMgr.GetCameraFromIndex(0, &guid); /* Default to first camera. */
	if(error != PGRERROR_OK)
	{
		PrintError(error);
		return -1;
	}

	error = cam.Connect(&guid);
	if(error != PGRERROR_OK)
	{
		PrintError(error);
		return -1;
	}

	error = cam.SetVideoModeAndFrameRate(VIDEOMODE_640x480Y8, FRAMERATE_30);
	if(error != PGRERROR_OK)
	{
		PrintError(error);
		return -1;
	}

	error = cam.GetCameraInfo(&camInfo);
	if(error != PGRERROR_OK)
	{
		PrintError(error);
		return -1;
	}

	error = cam.StartCapture();
	if(error != PGRERROR_OK)
	{
		PrintError(error);
		return -1;
	}

	printf("Camera opened successfully.\n");
	return 0;
}

int Firefly::close()
{
	error = cam.StopCapture();
	if(error != PGRERROR_OK)
	{
		PrintError(error);
		return -1;
	}

	error = cam.Disconnect();
	if(error != PGRERROR_OK)
	{
		PrintError(error);
		return -1;
	}
	printf("Camera closed.\n");
	return 0;
}

void Firefly::PrintError(Error error)
{
	error.PrintErrorTrace();
}

cv::Mat Firefly::getCVFrame()
{
	Image rawImage;
	Image BGRImage;
	cv::Mat image;

	if(!cam.IsConnected())
	{
		printf("Error: camera not connected.\n");
		return image;
	}

	error = cam.RetrieveBuffer(&rawImage);
	if(error != PGRERROR_OK)
	{
		PrintError(error);
		return image;
	}

	rawImage.Convert(PIXEL_FORMAT_BGR, &BGRImage);
	unsigned int rowBytes = BGRImage.GetReceivedDataSize()/BGRImage.GetRows();
	image = cv::Mat(BGRImage.GetRows(), BGRImage.GetCols(), CV_8UC3, BGRImage.GetData(), rowBytes);
	cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
	return image;
}
