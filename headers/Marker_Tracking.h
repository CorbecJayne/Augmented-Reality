#pragma once
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
#include "Player.h"
#include <tuple>

// Struct holding all infos about each strip, e.g. length
struct MyStrip {
	int stripeLength;
	int nStop;
	int nStart;
	cv::Point2f stripeVecX;
	cv::Point2f stripeVecY;
};

class Marker_Tracking {

public:
	// returns all currently detected markers
    std::tuple<cv::Mat,std::vector<Player>>detect_markers(cv::Mat&,const Question& question, int camera_width, int camera_height);

	cv::Mat calculate_Stripe(double dx, double dy, MyStrip & st);

	int subpixSampleSafe(const cv::Mat& pSrc, const cv::Point2f& p);

    void renderText(cv::Mat imgFiltered, std::string str, int x_start, int y_start,int size);
};