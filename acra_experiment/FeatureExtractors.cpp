#include "FeatureExtractors.hpp"

std::unique_ptr<std::vector<cv::Point2f>> Fast::extractFeatures(cv::Mat inputImage, int numPoints) {
	if(numPoints < 0) { numPoints = maxPoints; }
	std::vector<cv::KeyPoint> kPoints;
	std::unique_ptr<std::vector<cv::Point2f>> points(new std::vector<cv::Point2f>);

	cv::FAST(inputImage, kPoints, 30, true);

	for(auto kp : kPoints) {
		points->push_back(kp.pt);
	}

	return points;
}

std::unique_ptr<std::vector<cv::Point2f>> Harris::extractFeatures(cv::Mat inputImage, int numPoints) {
	if(numPoints < 0) { numPoints = maxPoints; }
	std::unique_ptr<std::vector<cv::Point2f>> points(new std::vector<cv::Point2f>);
	cv::goodFeaturesToTrack(inputImage, *points, numPoints, 0.01, 10, cv::Mat(), 3, 1, 0.04);
	return points;
}

std::unique_ptr<std::vector<cv::Point2f>> ShiTomasi::extractFeatures(cv::Mat inputImage, int numPoints) {
	if(numPoints < 0) { numPoints = maxPoints; }
	std::unique_ptr<std::vector<cv::Point2f>> points(new std::vector<cv::Point2f>);
	cv::goodFeaturesToTrack(inputImage, *points, numPoints, 0.01, 10, cv::Mat(), 3, 0, 0.04);
	return points;
}

std::unique_ptr<FeatureExtractor> getExtractor(std::string &exType) {
	FeatureExtractor *exRaw = NULL;

	if(exType == "fast")		{ exRaw = new Fast(); }
	else if(exType == "harris")	{ exRaw = new Harris(); }
	else if(exType == "shitomasi")	{ exRaw = new ShiTomasi(); }

	std::unique_ptr<FeatureExtractor> ex(exRaw);
	return ex;
}
