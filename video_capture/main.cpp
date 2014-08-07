#include <iomanip>
#include <opencv2/opencv.hpp>
#include <sstream>
#include "Firefly.hpp"

int main(int argc, char *argv[]) {
	// Check args
	if(argc != 3) {
		std::cout << "Usage: " << argv[0] << " framerate num_frames\n";
		return EXIT_FAILURE;
	}

	// Get framerate
	std::string framerate(argv[1]);

	// Get num_frames
	std::istringstream iss(argv[2]);
	int numFrames = 0;
	if(!(iss >> numFrames)) {
		std::cout << "Please enter a valid number for num_frames\n";
		return EXIT_FAILURE;
	}

	// Initialise FIrefly
	Firefly fly(framerate);

	cv::Mat frame;
	int frameCount = 0;
	std::vector<cv::Mat> frames((size_t)numFrames);

	std::cout << "Capturing frames...\n";
	while(frameCount < numFrames) {
		frame = fly.nextFrame();
		frames[frameCount] = frame;
		frameCount++;
	}

	std::stringstream filename;
	std::stringstream framenum;
	std::vector<int> params;
	frameCount = 0;

	params.push_back(cv::IMWRITE_PNG_COMPRESSION);
	params.push_back(0);

	std::cout << "Writing frames to disk...\n";
	for(auto fr : frames) {
		filename.str("");
		framenum.str("");
		framenum << std::setw(4) << std::setfill('0') << frameCount;
		filename << "frame" << framenum.str() << ".png";
		cv::imwrite(filename.str(), fr, params);
		frameCount++;
	}

	return EXIT_SUCCESS;
}
