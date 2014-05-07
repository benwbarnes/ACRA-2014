#include "Firefly.hpp"

Firefly::Firefly(libconfig::Config &config) : config(config), cam(), busMgr(), guid(), error(), bufferQueue(), queueMutex(), dataReady(), framerates()
{
	/* Populate framerate map */
	framerates["1.875"]	= FlyCapture2::FRAMERATE_1_875;
	framerates["3.75"]	= FlyCapture2::FRAMERATE_3_75;
	framerates["7.5"]	= FlyCapture2::FRAMERATE_7_5;
	framerates["15"]	= FlyCapture2::FRAMERATE_15;
	framerates["30"]	= FlyCapture2::FRAMERATE_30;
	framerates["60"]	= FlyCapture2::FRAMERATE_60;
	framerates["120"]	= FlyCapture2::FRAMERATE_120;
	framerates["240"]	= FlyCapture2::FRAMERATE_240;

	open();
	start();
}

Firefly::~Firefly()
{
	stop();
	close();
}

int Firefly::open()
{
	error = busMgr.GetCameraFromIndex(0, &guid); /* Default to first camera. */
	if(error != FlyCapture2::PGRERROR_OK)
	{
		throw std::runtime_error(std::string("Error finding camera: ") + error.GetDescription());
	}

	error = cam.Connect(&guid);
	if(error != FlyCapture2::PGRERROR_OK)
	{
		throw std::runtime_error(std::string("Error connecting to camera: ") + error.GetDescription());
	}

	std::string fr = config.lookup("camera.framerate");
	FlyCapture2::FrameRate framerate;

	try {
		framerate = framerates.at(fr);
	} catch(const std::out_of_range &oor) {
		throw std::runtime_error(std::string("camera/framerate = ") + fr + std::string(" is not a legal framerate."));
	}

	error = cam.SetVideoModeAndFrameRate(FlyCapture2::VIDEOMODE_640x480Y8, framerate);
	if(error != FlyCapture2::PGRERROR_OK)
	{
		throw std::runtime_error(std::string("Error setting video mode: ") + error.GetDescription());
	}
	return 0;
}

/* Note that the Firefly appears to use a Big endian architecture.
 * (so the bit positions here do not match the datasheet) */

int Firefly::start()
{
	uint32_t register_contents;

	bool externalTrigger = config.lookup("camera.externalTrigger");
	if(externalTrigger)
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

	/* Define a lambda expression to use as a callback. *
	 * ------------------------------------------------
	 * NOTE - unfortunately, it is impossible to capture [this] and simply call this->pushImage(),
	 * allowing pushImage() to be properly encapsulated.
	 * Here is why: the C++ standard dictates that a lambda expression with an empty capture list
	 * is compiled to a function, whereas one with a populated capture list compiles to a 'lambda' 
	 * class with an overloaded call operator. The compiled FlyCapture2 function will accept a
	 * function pointer, but it will not accept a lambda object. */

	auto onImageGet = [] (FlyCapture2::Image *pImage, const void *fly) { ((Firefly*) fly)->pushImage(pImage); };

	error = cam.StartCapture(onImageGet, this);
	if(error != FlyCapture2::PGRERROR_OK)
	{
		throw std::runtime_error(std::string("Error starting capture"));
	}
	return 0;
}

int Firefly::stop()
{
	error = cam.StopCapture();
	if(error != FlyCapture2::PGRERROR_OK)
	{
		error.PrintErrorTrace();
		return -1;
	}
	return 0;
}

int Firefly::close()
{
	error = cam.Disconnect();
	if(error != FlyCapture2::PGRERROR_OK)
	{
		error.PrintErrorTrace();
		return -1;
	}
	return 0;
}

void Firefly::pushImage(FlyCapture2::Image *rawImage)
{
	FlyCapture2::Image BGRImage;
	cv::Mat result;
	/* This code translates from FlyCapture2::Image to cv::Mat via their low-level data structures. */
	rawImage->Convert(FlyCapture2::PIXEL_FORMAT_BGR, &BGRImage);
	unsigned int rowBytes = BGRImage.GetReceivedDataSize()/BGRImage.GetRows();
	result = cv::Mat(BGRImage.GetRows(), BGRImage.GetCols(), CV_8UC3, BGRImage.GetData(), rowBytes);
	cv::cvtColor(result, result, cv::COLOR_BGR2GRAY);

	std::unique_lock<std::mutex> locker(queueMutex);
	bufferQueue.push(result);
	dataReady.notify_one();
}

cv::Mat Firefly::nextFrame()
{
	cv::Mat image;
	std::unique_lock<std::mutex> locker(queueMutex);

	while(bufferQueue.size() == 0)
	{
		dataReady.wait(locker);
	}

	image = bufferQueue.front();
	bufferQueue.pop();
	return image;
}
