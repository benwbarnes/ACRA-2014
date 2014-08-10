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
		std::cout << "Usage: " << argv[0] << " image_path num_points" << std::endl;
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
	int num_points = 0;
	if((iss >> num_points) == 0) {
		std::cout << "Please enter a valid number for num_points" << std::endl;
	}

	// Get Harris corners.
	std::vector<cv::Point2f> points_Harris;
	Timer timer_Harris;
	timer_Harris.start();
	cv::goodFeaturesToTrack(image, points_Harris, num_points, 0.01, 10, cv::Mat(), 3, 1, 0.04);
	timer_Harris.stop();

	// Get Shi-Tomasi corners.
	std::vector<cv::Point2f> points_ShiTomasi;
	Timer timer_ShiTomasi;
	timer_ShiTomasi.start();
	cv::goodFeaturesToTrack(image, points_ShiTomasi, num_points, 0.01, 10, cv::Mat(), 3, 0, 0.04);
	timer_ShiTomasi.stop();

	// Get FAST corners.
	std::vector<cv::Point2f> points_FAST;
	std::vector<cv::KeyPoint> keypoints_FAST;
	Timer timer_FAST;
	int FAST_Threshold = 9;
	do {
		FAST_Threshold++;
		points_FAST.clear();
		timer_FAST.start();
		cv::FAST(image, keypoints_FAST, FAST_Threshold, true);
		for(auto kp : keypoints_FAST) {
			points_FAST.push_back(kp.pt); // Grabs the 2D point data in isolation.
		}
		timer_FAST.stop();
	} while(keypoints_FAST.size() > num_points);

	std::cout << num_points << "\t";
	std::cout << points_Harris.size() << "\t" << timer_Harris.getTime() << "\t";
	std::cout << points_ShiTomasi.size() << "\t" << timer_ShiTomasi.getTime() << "\t";
	std::cout << points_FAST.size() << "\t" << timer_FAST.getTime() << std::endl;

	return EXIT_SUCCESS;
}

void imageToBGR(cv::Mat &image) {
	std::vector<cv::Mat> channels;
	for(int i = 0; i < 3; i++) channels.push_back(image);
	cv::merge(channels, image);
}

