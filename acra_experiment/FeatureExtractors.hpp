#ifndef FEATURE_EXTRACTOR_H
#define FEATURE_EXTRACTOR_H

#include <memory>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

enum ExtractorType {
	FAST,
	HARRIS,
	SHITOMASI
};

// Interface class which all feature extraction algorithms must implement.
class FeatureExtractor {
public:
	virtual ~FeatureExtractor() {};
	virtual std::unique_ptr<std::vector<cv::Point2f>> extractFeatures(cv::Mat, int) = 0;
	int maxPoints = 0;
};

class Fast : public FeatureExtractor {
public:
	std::unique_ptr<std::vector<cv::Point2f>> extractFeatures(cv::Mat, int);
};

class Harris : public FeatureExtractor {
public:
	std::unique_ptr<std::vector<cv::Point2f>> extractFeatures(cv::Mat, int);
};

class ShiTomasi : public FeatureExtractor {
public:
	std::unique_ptr<std::vector<cv::Point2f>> extractFeatures(cv::Mat, int);
};

std::unique_ptr<FeatureExtractor> getExtractor(std::string&);

#endif
