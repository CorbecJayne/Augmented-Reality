#pragma once
#include <opencv2/opencv.hpp>

class Player
{
	const int markerId;
	cv::Point2f positionPlayer1;
	int playerId;
	int time;
	bool locked_in;
	int points;

public:
	Player();
	~Player();
};

