#include <memory>
#include <sstream>
#include "FeatureExtractors.hpp"

#define MASK_KEEPOUT_SIZE 5 // Half the diagonal length of the rectangle used to mask off points when refreshing

void refreshPoints(const cv::Mat&, std::vector<cv::Point2f>&, const std::unique_ptr<FeatureExtractor>&, int);
void imageToBGR(cv::Mat&);

int main(int argc, char *argv[]) {

	// Check args
	if(argc != 3) {
		std::cout << "Usage: " << argv[0] << " image_folder  num_points" << std::endl;
		return EXIT_FAILURE;
	}

	// Get num_points
	std::istringstream ss(argv[2]);
	int numPoints = 0;
	if(!(ss >> numPoints)) {
		std::cout << "Please enter a valid number for num_points" << std::endl;
		return EXIT_FAILURE;
	}

	// Create file path for images
	std::string filePath(argv[1]);
	filePath.append("frame%04d.png");

	// Set up collection of extractors
	std::vector<std::unique_ptr<FeatureExtractor>> extractors;
	extractors.push_back(getExtractor("harris"));
	extractors.push_back(getExtractor("shitomasi"));
	extractors.push_back(getExtractor("fast"));

	// Get an initial image for extractors
	std::string initialImagePath(argv[1]);
	initialImagePath.append("frame0001.png");
	cv::Mat initialImage = cv::imread(initialImagePath, 0);

	// Set up a window for display
	cv::namedWindow("ACRA Experiment", cv::WINDOW_AUTOSIZE);

	// Set up termination criteria for KLT
	cv::TermCriteria termCrit(cv::TermCriteria::COUNT+cv::TermCriteria::EPS, 30, 0.01);

	for(unsigned int ex = 0; ex < extractors.size(); ex++) {

		// Set up structures for KLT
		cv::Mat firstImage, secondImage;
		std::vector<cv::Point2f> firstPoints, secondPoints;
		std::vector<uint8_t> status;
		std::vector<float> error;

		// Prepare video stream and images
		cv::VideoCapture video(filePath);
		video >> firstImage;
		video >> secondImage;

		// Copy set of initial points into the working set (firstPoints)
		std::unique_ptr<std::vector<cv::Point2f>> initialPoints = extractors[ex]->extractFeatures(initialImage, numPoints);
		for(auto pt : *initialPoints) {
			firstPoints.push_back(pt);
		}

		unsigned int frameNumber = 0;

		// Run KLT loop over images
		while(secondImage.data != NULL) {
			cv::calcOpticalFlowPyrLK(firstImage, secondImage, firstPoints, secondPoints, status, error, cv::Size(31, 31), 3, termCrit, 0, 0.001);

			for(unsigned int pt = 0; pt < status.size(); pt++) {
				if(status[pt] == 0) {
					secondPoints.erase(secondPoints.begin() + pt);
				}
			}

			firstPoints = secondPoints;
			firstImage = secondImage;

			refreshPoints(secondImage, secondPoints, extractors[ex], 50);

			// Display image with points
			imageToBGR(secondImage);
			for(auto pt : secondPoints) {
				cv::circle(secondImage, pt, 1, cv::Scalar(0, 255, 0), 2, 8, 0);
			}


			cv::imshow("ACRA Experiment", secondImage);
			cv::waitKey(1);

			std::cout << extractors[ex]->name << '\t' << frameNumber << '\t' << secondPoints.size() << std::endl;
			video >> secondImage;
			frameNumber++;
		}

		video.release();
	}

	return EXIT_SUCCESS;
}

void refreshPoints(const cv::Mat &image, std::vector<cv::Point2f> &currentPoints, const std::unique_ptr<FeatureExtractor> &ex, int numPoints) {
	// Construct mask
	cv::Point2f halfDiagonal(MASK_KEEPOUT_SIZE, MASK_KEEPOUT_SIZE);
	cv::Mat mask(image.size(), image.type(), cv::Scalar(255));
	for(auto pt : currentPoints) {
		cv::rectangle(mask, (pt - halfDiagonal), (pt + halfDiagonal), cv::Scalar(0), -1, 8, 0);
	}

	// Get vector of new points from extractor
	int newFeaturesRequired = numPoints - currentPoints.size();
	std::unique_ptr<std::vector<cv::Point2f>> newPoints = ex->extractFeatures(image, mask, newFeaturesRequired);
}

void imageToBGR(cv::Mat &image) {
	std::vector<cv::Mat> channels;
	for(int i = 0; i < 3; i++) channels.push_back(image);
	cv::merge(channels, image);
}