#pragma once
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
#include "Player.h"

// Struct holding all infos about each strip, e.g. length
struct MyStrip {
	int stripeLength;
	int nStop;
	int nStart;
	cv::Point2f stripeVecX;
	cv::Point2f stripeVecY;
};

class Marker_Tracking {
	
	int start_x;
	int start_y;
	int end_x;
	int end_y;

public:
	// returns all currently detected markers
	 std::vector<Player>detect_markers(cv::Mat);

	 cv::Mat calculate_Stripe(double dx, double dy, MyStrip & st);

	 int subpixSampleSafe(const cv::Mat& pSrc, const cv::Point2f& p);
	 
	 void calibrate();

};