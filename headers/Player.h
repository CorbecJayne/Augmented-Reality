#pragma once
#include <opencv2/opencv.hpp>

class Player
{
	const int markerId;
	cv::Point positionPlayer;
	int playerId;
	int time;
	bool locked_in;
	int points;
	int area;

public:
	Player();
	~Player();
	
	//getter and setter
	
	void add_points(int amount);
};