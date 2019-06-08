#pragma once
#include <opencv2/opencv.hpp>
#include <ctime>

class Player
{
	const int markerId;
	cv::Point positionPlayer;
	int playerId;
	time_t time;
	bool locked_in;
	int points;
	int area;


public:
	Player();
	~Player();
	
	//getter and setter
	
	void add_points(int amount);

	void set_time(time_t t);

    time_t getTime() const;

    int getArea() const;
};