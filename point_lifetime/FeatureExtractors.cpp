#include "FeatureExtractors.hpp"
#include <algorithm>

std::unique_ptr<std::vector<cv::Point2f>> Fast::extractFeatures(const cv::Mat &inputImage, const cv::Mat &mask, int numPoints) {
	if(numPoints < 0) { numPoints = maxPoints; }
	std::vector<cv::KeyPoint> kPoints;
	std::unique_ptr<std::vector<cv::Point2f>> points(new std::vector<cv::Point2f>);

	int FAST_Threshold = 40;
	cv::FAST(inputImage, kPoints, FAST_Threshold, true);

	// Sort the keypoints by their "strength" (response)
	std::sort(kPoints.begin(), kPoints.end(), [] (const cv::KeyPoint &kp1, const cv::KeyPoint &kp2) { return kp1.response > kp2.response; });

	// Copy the best points to the vector of new points, checking them against the mask.
	unsigned int index = 0;
	if(!mask.empty()) {
		while(points->size() < (unsigned int)numPoints && (unsigned int)index < kPoints.size()) {
			if(mask.at<float>(kPoints[index].pt.x, kPoints[index].pt.y) != 0) {
				points->push_back(kPoints[index].pt);
			}
			index++;
		}
	}
	else {
		while(points->size() < (unsigned int)numPoints) {
			points->push_back(kPoints[index].pt);
			index++;
		}
	}

	return points;
}

std::unique_ptr<std::vector<cv::Point2f>> Fast::extractFeatures(const cv::Mat &inputImage, int numPoints) {
	std::unique_ptr<std::vector<cv::Point2f>> points = extractFeatures(inputImage, cv::Mat(), numPoints);
	return points;
}

std::unique_ptr<std::vector<cv::Point2f>> Harris::extractFeatures(const cv::Mat &inputImage, const cv::Mat &mask, int numPoints) {
	std::unique_ptr<std::vector<cv::Point2f>> points(new std::vector<cv::Point2f>);
	cv::goodFeaturesToTrack(inputImage, *points, numPoints, 0.01, 10, mask, 3, 1, 0.04);
	return points;
}

std::unique_ptr<std::vector<cv::Point2f>> Harris::extractFeatures(const cv::Mat &inputImage, int numPoints) {
	if(numPoints < 0) { numPoints = maxPoints; }
	std::unique_ptr<std::vector<cv::Point2f>> points(new std::vector<cv::Point2f>);
	cv::goodFeaturesToTrack(inputImage, *points, numPoints, 0.01, 10, cv::Mat(), 3, 1, 0.04);
	return points;
}

std::unique_ptr<std::vector<cv::Point2f>> ShiTomasi::extractFeatures(const cv::Mat &inputImage, const cv::Mat &mask, int numPoints) {
	if(numPoints < 0) { numPoints = maxPoints; }
	std::unique_ptr<std::vector<cv::Point2f>> points(new std::vector<cv::Point2f>);
	cv::goodFeaturesToTrack(inputImage, *points, numPoints, 0.01, 10, mask, 3, 0, 0.04);
	return points;
}

std::unique_ptr<std::vector<cv::Point2f>> ShiTomasi::extractFeatures(const cv::Mat &inputImage, int numPoints) {
	if(numPoints < 0) { numPoints = maxPoints; }
	std::unique_ptr<std::vector<cv::Point2f>> points(new std::vector<cv::Point2f>);
	cv::goodFeaturesToTrack(inputImage, *points, numPoints, 0.01, 10, cv::Mat(), 3, 0, 0.04);
	return points;
}

Agast::Agast(const std::string &exName) :	FeatureExtractor(exName),
						brisk(30, 0, 1.0) {}

std::unique_ptr<std::vector<cv::Point2f>> Agast::extractFeatures(const cv::Mat &inputImage, const cv::Mat &mask, int numPoints) {
	if(numPoints < 0) { numPoints = maxPoints; }
	std::vector<cv::KeyPoint> kPoints;
	std::unique_ptr<std::vector<cv::Point2f>> points(new std::vector<cv::Point2f>);
	brisk(inputImage, mask, kPoints, cv::noArray(), false);

	std::sort(kPoints.begin(), kPoints.end(), [] (const cv::KeyPoint &kp1, const cv::KeyPoint &kp2) { return kp1.response > kp2.response; });

	unsigned int index = 0;
	while(points->size() < (unsigned int)numPoints) {
		points->push_back(kPoints[index].pt);
		index++;
	}

	return points;
}

std::unique_ptr<std::vector<cv::Point2f>> Agast::extractFeatures(const cv::Mat &inputImage, int numPoints) {
	return extractFeatures(inputImage, cv::Mat(), numPoints);
}

std::unique_ptr<FeatureExtractor> getExtractor(const std::string &exType) {
	FeatureExtractor *exRaw = NULL;

	if(exType == "fast")		{ exRaw = new Fast("FAST"); }
	else if(exType == "harris")	{ exRaw = new Harris("Harris"); }
	else if(exType == "shitomasi")	{ exRaw = new ShiTomasi("Shi Tomasi"); }
	else if(exType == "agast")	{ exRaw = new Agast("AGAST"); }

	std::unique_ptr<FeatureExtractor> ex(exRaw);
	return ex;
}

