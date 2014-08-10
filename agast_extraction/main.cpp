#include <opencv2/opencv.hpp>

void imageToBGR(cv::Mat&);

class Timer {
public:
	double initialTime = 0;
	double finalTime = 0;

	void start() {
		initialTime = (double)cv::getTickCount();
	}

	void stop() {
		finalTime = (double)cv::getTickCount();
	}

	double getTime() {
		double time = ((finalTime - initialTime)/cv::getTickFrequency());
		return time;
	}
};

int main(int argc, char *argv[]) {

	// Check for the correct number of command line arguments.
	if(argc != 3) {
		std::cout << "Usage: " << argv[0] << " image_path threshold" << std::endl;
		return EXIT_FAILURE;
	}

	// Load the image.
	cv::Mat image = cv::imread(argv[1], 0); // '0' parameter causes greyscale image to be loaded.
	if(image.data == NULL) {
		std::cout << "Unable to read image: " << argv[1] << std::endl;
		return EXIT_FAILURE;
	}

	// Load the number of points.
	std::istringstream iss(argv[2]);
	int threshold = 0;
	if((iss >> threshold) == 0) {
		std::cout << "Please enter a valid number for threshold" << std::endl;
	}

	// Get AGAST corners
	std::vector<cv::Point2f> points_AGAST;
	std::vector<cv::KeyPoint> keypoints_AGAST;
	Timer timer_AGAST;

	cv::BRISK brisk(threshold, 0, 1.0);
	timer_AGAST.start();
	brisk(image, cv::Mat(), keypoints_AGAST, cv::noArray(), false);
	for(auto kp : keypoints_AGAST) {
		points_AGAST.push_back(kp.pt);
	}
	timer_AGAST.stop();

	std::cout << points_AGAST.size() << "\t" << timer_AGAST.getTime() << std::endl;

	return EXIT_SUCCESS;
}
