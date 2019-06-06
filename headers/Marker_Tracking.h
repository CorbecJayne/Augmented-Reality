#pragma once
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

class Marker_Tracking {
	
	int start_x;
	int start_y;

public:
	// returns all currently detected markers
	 vector<Player>detect_markers(cv::Mat);
	 
	 void calibrate();

};