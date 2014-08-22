#ifndef FEATURE_EXTRACTOR_H
#define FEATURE_EXTRACTOR_H

#include <memory>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

#include "agast.h"

enum ExtractorType {
	FAST,
	HARRIS,
	SHITOMASI
};

// Interface class which all feature extraction algorithms must implement.
class FeatureExtractor {
public:
	FeatureExtractor(std::string exName) : name(exName) {};
	virtual ~FeatureExtractor() {};
	virtual std::unique_ptr<std::vector<cv::Point2f>> extractFeatures(const cv::Mat&, const cv::Mat&, int) = 0;
	virtual std::unique_ptr<std::vector<cv::Point2f>> extractFeatures(const cv::Mat&, int) = 0;
	int maxPoints = 0;
	const std::string name;
};

class Fast : public FeatureExtractor {
public:
	Fast(const std::string& exName) : FeatureExtractor(exName) {};
	std::unique_ptr<std::vector<cv::Point2f>> extractFeatures(const cv::Mat&, const cv::Mat&, int);
	std::unique_ptr<std::vector<cv::Point2f>> extractFeatures(const cv::Mat&, int);
};

class Harris : public FeatureExtractor {
public:
	Harris(const std::string& exName) : FeatureExtractor(exName) {};
	std::unique_ptr<std::vector<cv::Point2f>> extractFeatures(const cv::Mat&, const cv::Mat&, int);
	std::unique_ptr<std::vector<cv::Point2f>> extractFeatures(const cv::Mat&, int);
};

class ShiTomasi : public FeatureExtractor {
public:
	ShiTomasi(const std::string& exName) : FeatureExtractor(exName) {};
	std::unique_ptr<std::vector<cv::Point2f>> extractFeatures(const cv::Mat&, const cv::Mat&, int);
	std::unique_ptr<std::vector<cv::Point2f>> extractFeatures(const cv::Mat&, int);
};

class Agast : public FeatureExtractor {
public:
	Agast(const std::string&);
	std::unique_ptr<std::vector<cv::Point2f>> extractFeatures(const cv::Mat&, const cv::Mat&, int);
	std::unique_ptr<std::vector<cv::Point2f>> extractFeatures(const cv::Mat&, int);
private:
//	cv::BRISK brisk;
	cv::Ptr<cv::FeatureDetector> brisk;
};

class AgastNoCV : public FeatureExtractor {
public:
	AgastNoCV(const std::string&);
	std::unique_ptr<std::vector<cv::Point2f>> extractFeatures(const cv::Mat&, const cv::Mat&, int);
	std::unique_ptr<std::vector<cv::Point2f>> extractFeatures(const cv::Mat&, int);
private:
	std::unique_ptr<AgastDetector7_12s> agast7_12s;
};

std::unique_ptr<FeatureExtractor> getExtractor(const std::string&);

#endif
