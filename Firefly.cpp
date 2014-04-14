#include "Firefly.hpp"

Firefly::Firefly() : image(640, 480, CV_8UC3, 255), seqNum(-1) {};

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

	printf("Camera opened successfully.\n");
	return 0;
}

/* Note: this is a static callback function, not part of the Firefly class. */
void onImageGet(Image *pImage, const void *fly)
{
	Firefly &_fly = *(Firefly*)fly; /* Icky pointer casting to get the original object which called start() */
	_fly.pushImage(pImage);
}

int Firefly::start(int mode)
{
	uint32_t register_contents;
	/* Note that the Firefly appears to use a Big endian architecture. */
	/* (so the bit positions here do not match the datasheet) */
	if(mode == EXTERNAL)
	{
		/* Set up registers */
		cam.ReadRegister(0x830, &register_contents);
		register_contents |= (1<<25);
		register_contents &= ~(1<<16 | 1<<17 | 1<<18 | 1<<19);
		cam.WriteRegister(0x830, register_contents);
	}

	else
	{
		/* Set up registers */
		cam.ReadRegister(0x830, &register_contents);
		register_contents &= ~(1<<16 | 1<<17 | 1<<18 | 1<<19 | 1<<25);
		cam.WriteRegister(0x830, register_contents);
	}

	/* Start isochronous capture with callback. */
	error = cam.StartCapture(onImageGet, this);
	if(error != PGRERROR_OK)
	{
		PrintError(error);
		return -1;
	}
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

void Firefly::pushImage(Image *rawImage)
{
	Image BGRImage;
	cv::Mat result;
	rawImage->Convert(PIXEL_FORMAT_BGR, &BGRImage);
	unsigned int rowBytes = BGRImage.GetReceivedDataSize()/BGRImage.GetRows();
	result = cv::Mat(BGRImage.GetRows(), BGRImage.GetCols(), CV_8UC3, BGRImage.GetData(), rowBytes);
	cv::cvtColor(result, result, cv::COLOR_BGR2GRAY);
	image = result;
//	std::cout << "Seq: " << seqNum << std::endl;
	seqNum++;
}

int Firefly::getSeqNum()
{
	return seqNum;
}

cv::Mat Firefly::getImage()
{
	return image;
}
