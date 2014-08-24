#include <memory>
#include <sstream>
#include "FeatureExtractors.hpp"

#define MASK_KEEPOUT_SIZE 5 // Half the diagonal length of the rectangle used to mask off points when refreshing

std::unique_ptr<std::vector<cv::Point2f>> refreshPoints(const cv::Mat&, std::vector<cv::Point2f>&, const std::unique_ptr<FeatureExtractor>&, int);
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

	// Returns time in ms
	double getTime() {
		double time = ((1000*(finalTime - initialTime))/cv::getTickFrequency());
		return time;
	}
};

int main(int argc, char *argv[]) {

	// Check args
	if(argc != 5) {
		std::cout << "Usage: " << argv[0] << " image_folder  num_points tracking_threshold(0.001) pm_on[1|0]" << std::endl;
		return EXIT_FAILURE;
	}

	// Get num_points
	std::istringstream num_points_ss(argv[2]);
	int numPoints = 0;
	if(!(num_points_ss >> numPoints)) {
		std::cout << "Please enter a valid number for num_points" << std::endl;
		return EXIT_FAILURE;
	}

	// Get tracking threshold
	std::istringstream threshold_ss(argv[3]);
	float threshold = 0;
	if(!(threshold_ss >> threshold)) {
		std::cout << "Please enter a valid number for threshold" << std::endl;
		return EXIT_FAILURE;
	}

	// Get point_management_on
	std::istringstream pm_on_ss(argv[4]);
	int pm_on = 0;
	if(!(pm_on_ss >> pm_on)) {
		std::cout << "Please enter a valid number for pm_on (1 or 0)" << std::endl;
		return EXIT_FAILURE;
	}

	// Create file path for images
	std::string filePath(argv[1]);
	filePath.append("image%03d.png");

	// Set up collection of extractors
	std::vector<std::unique_ptr<FeatureExtractor>> extractors;
	extractors.push_back(getExtractor("harris"));
	extractors.push_back(getExtractor("shitomasi"));
	extractors.push_back(getExtractor("fast"));
	extractors.push_back(getExtractor("agast"));
	extractors.push_back(getExtractor("agastNoCV"));

	// Get an initial image for extractors
	std::string initialImagePath(argv[1]);
	initialImagePath.append("image001.png");
	cv::Mat initialImage = cv::imread(initialImagePath, 0);

	// Set up a window for display
	cv::namedWindow("ACRA Experiment", cv::WINDOW_AUTOSIZE);

	// Set up termination criteria for KLT
	cv::TermCriteria termCrit(cv::TermCriteria::COUNT+cv::TermCriteria::EPS, 30, 0.01);

	for(unsigned int ex = 0; ex < extractors.size(); ex++) {

		// Print .csv heading row
		std::cout << "Name" << '\t' << "frame_num" << '\t' << "pts_tracked" << '\t';
		if(pm_on) {
			std::cout << "pts_added" << '\t';
		}
		std::cout <<  "loop_time" << '\t' << "track_time";
		if(pm_on) {
			std::cout << '\t' << "mgmt_time";
		}
		std::cout << std::endl;

		// Set up structures for KLT
		cv::Mat firstImage, secondImage;
		std::vector<cv::Point2f> firstPoints, secondPoints;
		std::vector<uint8_t> status;
		std::vector<float> error;
		std::unique_ptr<std::vector<cv::Point2f>> newPoints;
		int num_points_added = 0;

		// Set up timers
		Timer full_loop_timer;
		Timer tracker_timer;
		Timer point_management_timer;

		// Prepare video stream and images
		cv::VideoCapture video(filePath);
		video >> firstImage;
		video >> secondImage;
//		cv::cvtColor(firstImage, firstImage, 6);
//		cv::cvtColor(secondImage, secondImage, 6);

		// Copy set of initial points into the working set (firstPoints)
		std::unique_ptr<std::vector<cv::Point2f>> initialPoints = extractors[ex]->extractFeatures(initialImage, numPoints);
		for(auto pt : *initialPoints) {
			firstPoints.push_back(pt);
		}

		unsigned int frameNumber = 0;

		// Run KLT loop over images
		while(secondImage.data != NULL) {
			full_loop_timer.start();
			tracker_timer.start();
			cv::calcOpticalFlowPyrLK(firstImage, secondImage, firstPoints, secondPoints, status, error, cv::Size(31, 31), 3, termCrit, cv::OPTFLOW_LK_GET_MIN_EIGENVALS, threshold);
			tracker_timer.stop();

			// Remove untracked points
			if(pm_on) {
				point_management_timer.start();
			}
			for(unsigned int pt = 0; pt < status.size(); pt++) {
				if(status[pt] == 0) {
					secondPoints.erase(secondPoints.begin() + pt);
				}
			}

			if((!pm_on) && secondPoints.size() == 0) break;

			firstPoints = secondPoints;
			firstImage = secondImage;

			// Point management
			if(pm_on) {
				newPoints = refreshPoints(secondImage, secondPoints, extractors[ex], numPoints);
				num_points_added = 0;
				if(newPoints != NULL) {
					for(auto pt : *newPoints) {
						firstPoints.push_back(pt);
					}
					num_points_added = newPoints->size();
					newPoints->clear();
				}
				point_management_timer.stop();
			}

			full_loop_timer.stop();
			// Display image with points
			imageToBGR(secondImage);
			for(auto pt : secondPoints) {
				cv::circle(secondImage, pt, 1, cv::Scalar(0, 255, 0), 2, 8, 0);
			}


			cv::imshow("ACRA Experiment", secondImage);
			cv::waitKey(1);

			std::cout << extractors[ex]->name << '\t' << frameNumber << '\t' << secondPoints.size();
			if(pm_on) {
				std::cout << '\t' << num_points_added;
			}
			std::cout << '\t' << full_loop_timer.getTime() << '\t' << tracker_timer.getTime();
			if(pm_on) {
				std::cout << '\t' << point_management_timer.getTime();
			}
			std::cout << std::endl;
			video >> secondImage;
//			cv::cvtColor(secondImage, secondImage, 6);
			frameNumber++;
		}

		video.release();
	}

	return EXIT_SUCCESS;
}

std::unique_ptr<std::vector<cv::Point2f>> refreshPoints(const cv::Mat &image, std::vector<cv::Point2f> &currentPoints, const std::unique_ptr<FeatureExtractor> &ex, int numPoints) {
	std::unique_ptr<std::vector<cv::Point2f>> newPoints;
	if(currentPoints.size() < (unsigned int)numPoints) {
		// Construct mask
		cv::Point2f halfDiagonal(MASK_KEEPOUT_SIZE, MASK_KEEPOUT_SIZE);
		cv::Mat mask(image.size(), image.type(), cv::Scalar(255));
		for(auto pt : currentPoints) {
			cv::rectangle(mask, (pt - halfDiagonal), (pt + halfDiagonal), cv::Scalar(0), -1, 8, 0);
		}

		// Get vector of new points from extractor
		int newFeaturesRequired = numPoints - currentPoints.size();
		newPoints = ex->extractFeatures(image, mask, newFeaturesRequired);
	}
	return newPoints;
}

void imageToBGR(cv::Mat &image) {
	std::vector<cv::Mat> channels;
	for(int i = 0; i < 3; i++) channels.push_back(image);
	cv::merge(channels, image);
}
