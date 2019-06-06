#pragma once
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

class Marker_Tracking {

public:
	// returns all currently detected markers
	 detect_markers(cv::Mat);

};