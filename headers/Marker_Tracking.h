#pragma once
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
#include "headers/Player.h"

class Marker_Tracking {
	
	int start_x;
	int start_y;
	int end_x;
	int end_y;

public:
	// returns all currently detected markers
	 std::vector<Player>detect_markers(cv::Mat);
	 
	 void calibrate();

};