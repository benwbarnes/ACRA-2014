#include <memory>
#include <sstream>
#include "FeatureExtractors.hpp"

void imageToBGR(cv::Mat&);

int main(int argc, char *argv[]) {

	if(argc != 3) {
		std::cout << "Usage: " << argv[0] << " extractor_type num_points" << std::endl;
		std::cout << "Available extractor types: fast, harris, shitomasi" << std::endl;
		return EXIT_FAILURE;
	}

	std::string exType(argv[1]);
	std::unique_ptr<FeatureExtractor> ex = getExtractor(exType);

	if(ex == NULL) {
		std::cout << "Please enter a valid descriptor type" << std::endl;
	}

	std::istringstream ss(argv[2]);
	int numPoints = 0;
	if(!(ss >> numPoints)) {
		std::cout << "Please enter a valid number for num_points" << std::endl;
		return EXIT_FAILURE;
	}

	ex->maxPoints = numPoints;

	cv::namedWindow("ACRA Experiment", cv::WINDOW_AUTOSIZE);
	cv::VideoCapture cap("images/frame%04d.png");
	cv::Mat image, prevImage;

	std::vector<uint8_t> status;
	std::vector<float> error;

	std::vector<cv::Point2f> points;
	std::vector<cv::Point2f> nextPoints;
	std::unique_ptr<std::vector<cv::Point2f>> newPoints(new std::vector<cv::Point2f>);

	char keyPressed = '\0';
	int activePoints = 0;

	cap >> prevImage;
//	cv::cvtColor(prevImage, prevImage, cv::COLOR_BGR2GRAY); // Remove when using pre-converted video stream

	newPoints = ex->extractFeatures(prevImage, numPoints);
	for(auto pt : *newPoints) {
		points.push_back(pt);
	}

	while(keyPressed != 'q') {
		cap >> image;
//		cv::cvtColor(image, image, cv::COLOR_BGR2GRAY); // Remove when using pre-converted video stream

		activePoints = points.size();
		if(activePoints < numPoints) {
			newPoints = ex->extractFeatures(image, numPoints - activePoints);
			for(auto pt : *newPoints) {
				points.push_back(pt);
				std::cout << "Added a point" << std::endl;
			}
		}

		cv::calcOpticalFlowPyrLK(prevImage, image, points, nextPoints, status, error, cv::Size(31, 31), 3, cv::TermCriteria(cv::TermCriteria::COUNT+cv::TermCriteria::EPS, 30, 0.01), 0, 0.001);

		for(uint i = 0; i < status.size(); i++) {
			if(status[i] == 0) {
				nextPoints.erase(points.begin() + 5 );
				std::cout << "Erasing a point" << std::endl;
			}
		}

		points = nextPoints;
		prevImage = image;

		imageToBGR(image);

		for(auto pt : points) {
			cv::circle(image, pt, 1, cv::Scalar(255, 0, 255), 1, 8, 0);
		}

		cv::imshow("ACRA Experiment", image);
		keyPressed = cv::waitKey(1);
	}

	return EXIT_SUCCESS;
}

void imageToBGR(cv::Mat &image) {
	std::vector<cv::Mat> channels;
	for(int i = 0; i < 3; i++) channels.push_back(image);
	cv::merge(channels, image);
}

void createPointMask(const cv::Mat &image, cv::Mat &mask, const std::vector<cv::Point2f> &points) {
//	mask = cv::Mat(640, 480, cv::8UC1);
//	for(auto pt : points) {
//
//	}
}
